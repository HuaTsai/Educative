// Build: -ltbb is required
#include <bits/stdc++.h>

#include <execution>

using namespace std;
using namespace std::chrono_literals;

void sleepms(const unsigned ms) {
  this_thread::sleep_for(chrono::milliseconds(ms));
}

void fthread(const int n) {
  cout << format("This is t{}\n", n);
  for (size_t i{}; i < 5; ++i) {
    sleepms(100 * n);
    cout << format("t{}: {}\n", n, i + 1);
  }
  cout << format("Finishing t{}\n", n);
}

struct prime_time {
  chrono::duration<double> dur{};
  uint64_t count{};
};

prime_time count_primes(const uint64_t &n) {
  prime_time ret{};
  constexpr auto isprime = [](const uint64_t &n) {
    for (uint64_t i{2}; i < n / 2; ++i)
      if (n % i == 0) return false;
    return true;
  };
  auto t1 = chrono::steady_clock::now();
  for (uint64_t i{2}; i <= n; ++i)
    if (isprime(i)) ++ret.count;
  ret.dur = chrono::steady_clock::now() - t1;
  return ret;
}

void f(promise<int> value) {
  cout << "this is f()\n";
  value.set_value(42);
}

mutex animal_mutex;

struct Animal {
  string_view name{"unknown"};
  list<Animal> friends;
  Animal() = delete;
  Animal(string_view n) : name{n} {}

  bool operator==(const Animal &o) const {
    return name.data() == o.name.data();
  }

  bool is_friend(const Animal &o) const {
    for (const auto &a : friends)
      if (a == o) return true;
    return false;
  }

  optional<list<Animal>::iterator> find_friend(const Animal &o) {
    for (auto it = friends.begin(); it != friends.end(); ++it)
      if (*it == o) return it;
    return nullopt;
  }

  void print() const noexcept {
    lock_guard<mutex> lock(animal_mutex);
    int n = friends.size();
    cout << format("Animal: {}, friends: ", name);
    if (!n) {
      cout << "none\n";
    } else {
      for (const auto &a : friends) {
        cout << a.name;
        if (--n) cout << ", ";
      }
      cout << "\n";
    }
  }

  bool add_friend(Animal &o) noexcept {
    cout << format("add friend: {} -> {}\n", name, o.name);
    if (*this == o) return false;
    lock_guard<mutex> lock(animal_mutex);
    if (!is_friend(o)) friends.emplace_back(o);
    if (!o.is_friend(*this)) o.friends.emplace_back(*this);
    return true;
  }

  bool delete_friend(Animal &o) noexcept {
    cout << format("delete friend: {} -> {}\n", name, o.name);
    if (*this == o) return false;
    lock_guard<mutex> lock(animal_mutex);
    if (auto it = find_friend(o)) friends.erase(it.value());
    if (auto it = o.find_friend(*this)) o.friends.erase(it.value());
    return true;
  }
};

// atomic: usually in global scope for threads to access
atomic_bool ready{};        // alias of atomic<bool>
atomic_uint64_t g_count{};  // alias of atomic<uint64_t>
atomic_flag winner{};

// condition_variable
deque<size_t> q{};
mutex mtx{};
condition_variable cond{};
bool finished{};

void producer() {
  for (size_t i{}; i < 10; ++i) {
    this_thread::sleep_for(200ms);
    lock_guard<mutex> x{mtx};
    q.push_back(i);
    // key point: notify_all will wake up threads for them to aquire the lock
    cond.notify_all();
  }
  lock_guard<mutex> x{mtx};
  finished = true;
  cond.notify_all();
}

void consumer() {
  while (!finished) {
    // must be unique_lock
    unique_lock<mutex> lock{mtx};
    // key point: wait will release the lock for producer and block the thread
    cond.wait(lock, [] { return !q.empty() || finished; });
    if (!q.empty()) {
      cout << format("consume {}\n", q.front());
      q.pop_front();
    }
  }
}

int main() {
  // std::this_thread::sleep_for takes a duration as argument
  // std::this_thread::sleep_until takes a time_point as argument
  auto t1 = chrono::steady_clock::now();
  cout << "sleep for 1.3 seconds\n";
  this_thread::sleep_for(1s + 300ms);
  cout << "sleep for 2 seconds\n";
  this_thread::sleep_until(chrono::steady_clock::now() + 2s);
  chrono::duration<double> dur1 = chrono::steady_clock::now() - t1;
  cout << format("total duration: {:.5}s\n", dur1.count());

  // POSIX standard: unsigned int sleep(unsigned int seconds)

  // std::threads: join will wait while detach will not
  // thread and object are one-to-one relationship, so thread cannot be copied
  // but can be moved
  thread th1(fthread, 1);
  th1.join();

  th1 = thread(fthread, 1);
  thread th2(fthread, 2);
  th1.detach();
  th2.detach();
  cout << "main() sleep 2 seconds\n";
  sleepms(2000);

  // C++20: std::jthread, automatically joins the caller at the end of its
  // scope
  jthread jt1(fthread, 1);

  // std::async, similar to std::thread but returns a future object
  constexpr uint64_t MAX_PRIME{0x1FFFF};
  auto primes1 = async(count_primes, MAX_PRIME);
  auto pt = primes1.get();  // block the future object until completed
  cout << format("primes: {} {:.3}\n", pt.count, pt.dur);

  // launch::async enables asynchronous operation
  // launch::deferred enables lazy evaluation
  list<std::future<prime_time>> swarm;
  cout << "start parallel primes\n";
  auto t2{chrono::steady_clock::now()};
  for (size_t i{}; i < 16; ++i)
    swarm.emplace_back(async(launch::async, count_primes, MAX_PRIME));
  for (auto &f : swarm) {
    static size_t i{};
    auto pt = f.get();
    cout << format("primes({:02}): {} {:.5}\n", ++i, pt.count, pt.dur);
  }
  auto secs = chrono::duration<double>(chrono::steady_clock::now() - t2);
  cout << format("total duration: {:.5}s\n", secs.count());

  // promise serves as a bridge to future object
  // promise can only be moved, not copied
  // async returns a future object, which simplifies the creation of promise
  promise<int> value_promise;
  future<int> value_future = value_promise.get_future();
  thread th3(f, std::move(value_promise));
  th3.detach();
  cout << format("value is {}\n", value_future.get());

  // executaion policy
  vector<unsigned> v(1'000'000);
  random_device rng;

  for (auto &i : v) i = rng() % 0xFFFF;
  auto t3{chrono::steady_clock::now()};
  // <execution> is commented in <bits/stdc++.h>, use -ltbb to compile it
  transform(execution::seq, v.begin(), v.end(), v.begin(),
            [](unsigned n) { return n * 2; });
  auto ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t3);
  cout << format("no policy: {:.3}ms\n", ms.count());

  for (auto &i : v) i = rng() % 0xFFFF;
  auto t4{chrono::steady_clock::now()};
  transform(execution::par, v.begin(), v.end(), v.begin(),
            [](unsigned n) { return n * 2; });
  auto ms2 = chrono::duration<double, milli>(chrono::steady_clock::now() - t4);
  cout << format("par policy: {:.3}ms\n", ms2.count());

  for (auto &i : v) i = rng() % 0xFFFF;
  auto t5{chrono::steady_clock::now()};
  transform(execution::par_unseq, v.begin(), v.end(), v.begin(),
            [](unsigned n) { return n * 2; });
  auto ms3 = chrono::duration<double, milli>(chrono::steady_clock::now() - t5);
  cout << format("par_unseq policy: {:.3}ms\n", ms3.count());

  // mutex, lock_guard
  // more: shared_mutex, recursive_mutex, timed_mutex
  auto cat1 = std::make_unique<Animal>("Cat");
  auto tiger1 = std::make_unique<Animal>("Tiger");
  auto dog1 = std::make_unique<Animal>("Dog");
  auto rabbit1 = std::make_unique<Animal>("Rabbit");

  auto a1 = std::async([&] { cat1->add_friend(*tiger1); });
  auto a2 = std::async([&] { cat1->add_friend(*rabbit1); });
  auto a3 = std::async([&] { rabbit1->add_friend(*dog1); });
  auto a4 = std::async([&] { rabbit1->add_friend(*cat1); });
  a1.wait();
  a2.wait();
  a3.wait();
  a4.wait();

  auto p1 = std::async([&] { cat1->print(); });
  auto p2 = std::async([&] { tiger1->print(); });
  auto p3 = std::async([&] { dog1->print(); });
  auto p4 = std::async([&] { rabbit1->print(); });
  p1.wait();
  p2.wait();
  p3.wait();
  p4.wait();

  auto a5 = std::async([&] { cat1->delete_friend(*rabbit1); });
  a5.wait();
  auto p5 = std::async([&] { cat1->print(); });
  auto p6 = std::async([&] { rabbit1->print(); });

  // atomic: lock-free, encapsulates an object to synchronize access
  // test_and_set: set the flag to true and return the previous value
  constexpr int max_count{1'000'000};
  constexpr int max_threads{100};
  auto countem = [&](int id) {
    while (!ready) this_thread::yield();
    for (int i{}; i < max_count; ++i) ++g_count;
    if (!winner.test_and_set()) cout << format("thread {:02} won!\n", id);
  };
  vector<thread> swarm2;
  cout << format("spawn {} threads\n", max_threads);
  for (int i{}; i < max_threads; ++i) swarm2.emplace_back(countem, i);
  ready = true;  // std::latch and std::barrier may be better
  for (auto &t : swarm2) t.join();
  cout << format("global count: {}\n", to_string(g_count));

  // check whether the implementation is lock-free
  cout << format("is g_count lock-free? {}\n", g_count.is_lock_free());

  // Aliases for special-purpose types: integral atomic type that is lock-free
  // and for which waiting/notifying is most efficient
  // std::atomic_signed_lock_free
  // std::atomic_unsigned_lock_free

  // std::atomic_flag: the lock-free boolean atomic type

  // memory_orders: complicated and need to be wrap up

  // call_once: in <mutex>
  constexpr int max_threads2{25};
  once_flag init_flag;
  auto do_init = [](size_t id) { cout << format("do_init ({}): ", id); };
  auto do_print = [&](size_t id) {
    call_once(init_flag, do_init, id);
    cout << format("{} ", id);
  };
  list<thread> swarm3;
  for (size_t i{}; i < max_threads2; ++i) swarm3.emplace_back(do_print, i);
  for (auto &t : swarm3) t.join();
  cout << endl;

  // condition_variable, producer-consumer
  thread th4{producer};
  thread th5{consumer};
  th4.join();
  th5.join();
  cout << "finished!\n";
}
