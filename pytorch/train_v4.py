n_epochs = 200
losses = []
val_losses = []
for epoch in range(n_epochs):
    loss = mini_batch(device, train_loader, train_step)
    losses.append(loss)
    with torch.no_grad():
        val_loss = mini_batch(device, val_loader, val_step)
        val_losses.append(val_loss)
