import os
import numpy as np
import torch
import torch.optim as optim
import torch.nn as nn
from torch.utils.data import DataLoader, TensorDataset, random_split
from torch.utils.data.distributed import DistributedSampler
from torch.nn.parallel import DistributedDataParallel
from torch.distributed import init_process_group, destroy_process_group


class Trainer(object):
    def __init__(self, model, loss_fn, optimizer, local_rank, distributed=False):
        print("local rank: ", local_rank)
        self.model = model
        self.loss_fn = loss_fn
        self.optimizer = optimizer
        self.local_rank = local_rank
        self.distributed = distributed
        self.model.to(local_rank)  # still necessary before DDP
        if distributed:
            self.model = DistributedDataParallel(
                self.model, device_ids=[local_rank], output_device=local_rank
            )
        self.total_epochs = 0
        self.train_step = self._make_train_step()
        self.val_step = self._make_val_step()

    def set_loaders(self, train_loader, val_loader):
        self.train_loader = train_loader
        self.val_loader = val_loader

    def set_samplers(self, train_sampler, val_sampler):
        self.train_sampler = train_sampler
        self.val_sampler = val_sampler

    def train(self, n_epochs):
        for epoch in range(n_epochs):
            if self.distributed:
                self.train_sampler.set_epoch(epoch)
                self.val_sampler.set_epoch(epoch)
            self.total_epochs += 1
            loss = self._mini_batch(validation=False)
            with torch.no_grad():
                val_loss = self._mini_batch(validation=True)
            print(
                f"[GPU {self.local_rank}] {self.total_epochs:03d} train loss: {loss:.6f}, val loss: {val_loss:.6f}"
            )

    def predict(self, x):
        self.model.eval()
        x_tensor = torch.as_tensor(x).float()
        y_hat_tensor = self.model(x_tensor.to(self.local_rank))
        self.model.train()
        return y_hat_tensor.detach().cpu().numpy()

    def _make_train_step(self):
        def perform_train_step(x, y):
            self.model.train()
            yhat = self.model(x)
            loss = self.loss_fn(yhat, y)
            loss.backward()
            self.optimizer.step()
            self.optimizer.zero_grad()
            return loss.item()

        return perform_train_step

    def _make_val_step(self):
        def perform_val_step(x, y):
            self.model.eval()
            yhat = self.model(x)
            loss = self.loss_fn(yhat, y)
            return loss.item()

        return perform_val_step

    def _mini_batch(self, validation=False):
        if validation:
            data_loader = self.val_loader
            step = self.val_step
        else:
            data_loader = self.train_loader
            step = self.train_step

        mini_batch_losses = []
        for x_batch, y_batch in data_loader:
            x_batch = x_batch.to(self.local_rank)  # Put data to device
            y_batch = y_batch.to(self.local_rank)
            mini_batch_loss = step(x_batch, y_batch)
            mini_batch_losses.append(mini_batch_loss)
        loss = np.mean(mini_batch_losses)
        return loss


if __name__ == "__main__":
    # Set Distributed Training
    world_size = int(os.environ["WORLD_SIZE"])
    local_rank = int(os.environ["LOCAL_RANK"])

    distributed = world_size > 1
    print("Distributed: ", distributed)

    if distributed:
        init_process_group("nccl")
        torch.cuda.set_device(local_rank)

    # Set Seeds
    np.random.seed(42)
    torch.backends.cudnn.deterministic = True
    torch.backends.cudnn.benchmark = False
    torch.manual_seed(42)

    # Data Generation
    true_b, true_w, total_n = 1, 2, 100
    x = np.random.rand(total_n, 1)
    y = true_b + true_w * x + (0.1 * np.random.randn(total_n, 1))

    # Data Preparation
    train_n, val_n = 80, 20
    batch_size = 16
    x_tensor = torch.as_tensor(x).float()
    y_tensor = torch.as_tensor(y).float()
    dataset = TensorDataset(x_tensor, y_tensor)
    train_data, val_data = random_split(dataset, [train_n, val_n])

    if distributed:
        train_sampler = DistributedSampler(train_data)
        train_loader = DataLoader(train_data, batch_size, sampler=train_sampler)
        val_sampler = DistributedSampler(val_data)
        val_loader = DataLoader(val_data, batch_size, sampler=val_sampler)
    else:
        train_loader = DataLoader(train_data, batch_size, True)
        val_loader = DataLoader(val_data, batch_size)

    # Model, Optimizer, Loss
    model = nn.Sequential(nn.Linear(1, 1))
    loss_fn = nn.MSELoss(reduction="mean")
    optimizer = optim.SGD(model.parameters(), lr=0.1 * world_size)

    # Train
    trainer = Trainer(model, loss_fn, optimizer, local_rank, distributed)
    trainer.set_loaders(train_loader, val_loader)
    if distributed:
        trainer.set_samplers(train_sampler, val_sampler)
    trainer.train(50)

    if distributed:
        destroy_process_group()

    print(model.state_dict())
