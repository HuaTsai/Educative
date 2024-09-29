#include <bits/stdc++.h>

using namespace std;

optional<int> factor(int n) {
  for (int i = 2; i <= n / 2; ++i) {
    if (n % i == 0) return i;
  }
  return nullopt;  // {} also works
}

void p_any(const any &a) {
  if (!a.has_value()) {
    cout << "None.\n";
  } else if (a.type() == typeid(int)) {
    cout << format("int: {}\n", any_cast<int>(a));
  } else if (a.type() == typeid(string)) {
    cout << format("string: \"{}\"\n", any_cast<const string &>(a));
  } else if (a.type() == typeid(list<int>)) {
    cout << "list<int>: ";
    for (auto &i : any_cast<const list<int> &>(a)) cout << format("{} ", i);
    cout << '\n';
  } else {
    cout << format("something else: {}\n", a.type().name());
  }
}

class Animal {
  string_view name_;
  string_view sound_;

 public:
  explicit Animal(string_view n, string_view s) : name_(n), sound_(s) {}
  void speak() const { cout << format("{} says {}\n", name_, sound_); }
  void sound(string_view s) { sound_ = s; }
};

class Cat : public Animal {
 public:
  Cat(string_view n) : Animal(n, "meow") {}
};

class Dog : public Animal {
 public:
  Dog(string_view n) : Animal(n, "arf!") {}
};

class Wookie : public Animal {
 public:
  Wookie(string_view n) : Animal(n, "grrraarrgghh!") {}
};

using v_animal = std::variant<Cat, Dog, Wookie>;

struct animal_speaks {
  void operator()(const Dog &d) const { d.speak(); }
  void operator()(const Cat &c) const { c.speak(); }
  void operator()(const Wookie &w) const { w.speak(); }
};

// variadic template
template <typename T>
void f(T final) {
  cout << final << endl;
}

template <typename T, typename... Args>
void f(T first, Args... args) {
  cout << first;
  f(args...);
}

// Fold expressions
// Unary right fold: (pack op ...)
// Unary left fold: (... op pack)
// Binary right fold: (pack op ... op init)
// Binary left fold: (init op ... op pack) *
template <typename... Args>
void g(Args... args) {
  (cout << ... << args) << endl;
}

// index_sequence is used to get the index of a tuple with unknown size
template <typename... T>
constexpr void print_t(const tuple<T...> &tup) {
  auto lpt = [&tup]<size_t... I>(std::index_sequence<I...>) constexpr {
    (..., (cout << format((I ? ", {}" : "{}"), get<I>(tup))));
    cout << '\n';
  };
  lpt(std::make_index_sequence<sizeof...(T)>());
}

template <typename... T>
constexpr int sum_t(const tuple<T...> &tup) {
  int accum{};
  auto lpt = [&tup, &accum]<size_t... I>(std::index_sequence<I...>) constexpr {
    (..., (accum += get<I>(tup)));
  };
  lpt(std::make_index_sequence<sizeof...(T)>());
  return accum;
}

// with apply
// template <typename... T>
// constexpr int sum_t(const tuple<T...> &tup) {
//   return std::apply([](const auto &...args) { return (args + ...); }, tup);
// }

template <typename RNG>
void histogram(const string_view &rng_name) {
  constexpr size_t n_samples{100000};
  constexpr size_t n_partitions{10};
  constexpr size_t n_max{50};
  auto p_ratio = (double)RNG::max() / n_partitions;
  RNG rng{};  // construct the engine object
  // collect the samples
  vector<size_t> v(n_partitions);

  for (size_t i{}; i < n_samples; ++i) {
    ++v[rng() / p_ratio];
  }
  // display the histogram
  auto max_el = std::max_element(v.begin(), v.end());
  auto v_ratio = *max_el / n_max;
  if (v_ratio < 1) v_ratio = 1;
  cout << format("engine: {}\n", rng_name);
  for (size_t i{}; i < n_partitions; ++i) {
    cout << format("{:02}:{:*<{}}\n", i + 1, ' ', v[i] / v_ratio);
  }
  cout << '\n';
}

void dist_histogram(auto distro, const string_view &dist_name) {
  constexpr size_t n_samples{10 * 1000};
  constexpr size_t n_max{50};
  std::default_random_engine rng{};
  map<long, size_t> m;
  // create the histogram map
  for (size_t i{}; i < n_samples; ++i) ++m[(long)distro(rng)];
  // print the histogram
  auto max_elm_it = max_element(
      m.begin(), m.end(),
      [](const auto &a, const auto &b) { return a.second < b.second; });

  size_t max_elm = max_elm_it->second;
  size_t max_div = std::max(max_elm / n_max, size_t(1));
  cout << format("{}:\n", dist_name);

  for (const auto [randval, count] : m) {
    if (count < max_elm / n_max) continue;
    cout << format("{:3}:{:*<{}}\n", randval, ' ', count / max_div);
  }
}

int main() {
  // std::optional in C++17, return value pointer or nullopt
  int a{42};
  int b{73};
  auto x = factor(a);
  auto y = factor(b);
  // x equals x.has_value(), *x equals x.value()
  // Note that if x does not have a value, then
  // 1) *x is undefined
  // 2) x.value() throws an exception
  if (x)
    cout << format("lowest factor of {} is {}\n", a, *x);
  else
    cout << format("{} is prime\n", a);
  if (y)
    cout << format("lowest factor of {} is {}\n", b, *y);
  else
    cout << format("{} is prime\n", b);
  x.reset();  // no longer has a value
  try {
    cout << x.value() << endl;  // x.value() may throw while *x does not
  } catch (const bad_optional_access &e) {
    cout << "x has no value\n";
  }

  // std::any
  any z = 42;
  if (z.has_value()) {
    cout << format("z has type: {}\n", z.type().name());
    cout << format("z has value: {}\n", any_cast<int>(z));
  } else {
    cout << "no value\n";
  }
  z = "abc"s;
  cout << format("z has type: {} with value {}\n", z.type().name(),
                 any_cast<string>(z));
  p_any({});
  p_any(47);
  p_any("abc"s);
  p_any(any(list{1, 2, 3}));
  p_any(any(vector{1, 2, 3}));
  // any has emplate(), reset(), has_value(), type(), operator=(), ...
  // any non-member functions: swap(), any_caast<T>(), ...
  // any_cast<T>() returns a copy, use T& for reference
  // any_cast<T>() throws bad_any_cast if the type is wrong

  // old fashioned union for ipv4
  union ipv4 {
    struct {
      uint8_t a, b, c, d;
    } quad;
    uint32_t i;
  };
  union ipv4 ip;
  ip.i = 0x2A05A8C0;
  cout << format("ip addr dotted quad: {}.{}.{}.{}\n", ip.quad.a, ip.quad.b,
                 ip.quad.c, ip.quad.d);
  cout << format("ip addr int32 (LE): {:08X}\n", ip.i);

  // std::variant, tagged union, C++17
  list<v_animal> pets{Cat{"Hobbes"}, Dog{"Fido"}, Cat{"Max"}, Wookie{"Chewie"}};

  // visit in <variant> is a function template that accepts a fuctor and variant
  // visit(f, v) calls f(v), the benifits is that no if-else is needed
  for (const auto &p : pets) {
    visit(animal_speaks{}, p);
  }

  // or use index() with get<>()
  for (const auto &p : pets) {
    if (p.index() == 0)
      get<Cat>(p).speak();
    else if (p.index() == 1)
      get<Dog>(p).speak();
    else if (p.index() == 2)
      get<Wookie>(p).speak();
  }

  // or use get_if<>() which returns a pointer
  for (const auto &p : pets) {
    if (auto cat = get_if<Cat>(&p))
      cat->speak();
    else if (auto dog = get_if<Dog>(&p))
      dog->speak();
    else if (auto wookie = get_if<Wookie>(&p))
      wookie->speak();
  }

  // holds_alternative<>() returns true/false
  for (const auto &p : pets) {
    if (holds_alternative<Cat>(p))
      get<Cat>(p).speak();
    else if (holds_alternative<Dog>(p))
      get<Dog>(p).speak();
    else if (holds_alternative<Wookie>(p))
      get<Wookie>(p).speak();
  }

  // chrono
  auto t1 = chrono::system_clock::now();
  cout << format("system_clock::now is {}\n", t1);
  cout << format("system_clock::now is {:%F %T}\n", t1);
  std::time_t t1_t = chrono::system_clock::to_time_t(t1);
  cout << "system_clock::now is "
       << std::put_time(std::localtime(&t1_t), "%F %T") << '\n';

  sleep(1);
  auto t2 = chrono::system_clock::now();
  // chrono::seconds -> chrono::duration<int64_t>
  chrono::duration<double> secs{t2 - t1};
  cout << format("time elapsed: {:.3f} seconds", secs.count());

  // template <class Rep, class Period = std::ratio<1>>
  // Period: how many seconds is a count
  //   ratio<1e18> -> exa
  //   ratio<1> -> seconds
  //   ratio<1, 1000> -> milliseconds
  //   ratio<1, 1000000> -> microseconds
  //   ratio<1, 1e18> -> atto
  // Rep: type of count() returns
  // for convenience, I suggest using chrono::duration<double, ...>

  // for timeing purpose, steady_clock is preferred for its monotonic ticks
  // another option is high_resolution_clock, which provides the shortest tick
  // period available on a given system but is not implemented consistently
  // across different implementations. It may be an alias for system_clock or
  // steady_clock, and it may or may not be monotonic. high_resolution_clock is
  // not recommended for general-purpose use.

  cout << format("time elapsed: {:.3f} sec\n", secs.count());
  cout << format("time elapsed: {:.3f} ms\n",
                 chrono::duration<double, milli>(secs).count());
  cout << format("time elapsed: {:.3e} μs\n",
                 chrono::duration<double, micro>(secs).count());
  cout << format("time elapsed: {} frames at 24 fps\n",
                 floor<chrono::duration<int64_t, ratio<1, 24>>>(secs).count());

  // tuple, advantage is that it can be used with fold expressions
  f("hello", " ", 47, " ", "world");
  g("hello", " ", 47, " ", "world");

  tuple lables{"ID", "Name", "Scale"};
  tuple employee{123456, "John Doe", 3.7};
  tuple nums{1, 7, "forty-two", 47, 73L, -111.11};
  print_t(lables);
  print_t(employee);
  print_t(nums);

  tuple ti1{1, 2, 3, 4, 5};
  tuple ti2{9, 10, 11, 12, 13, 14, 15};
  tuple ti3{47, 73, 42};
  auto sum1{sum_t(ti1)};
  auto sum2{sum_t(ti2)};
  auto sum3{sum_t(ti3)};
  cout << format("sum of ti1: {}\n", sum1);
  cout << format("sum of ti2: {}\n", sum2);
  cout << format("sum of ti3: {}\n", sum3);

  // smart pointers
  // a key difference between unique_ptr and shared_ptr is that unique_ptr
  // cannot be copied, i.e., f(unique_ptr<int> p) is illegal while f(const
  // unique_ptr<int> &p) is legal.
  unique_ptr<int> p1{new int{42}};
  cout << format("p1: {} @ {}\n", *p1, static_cast<void *>(p1.get()));
  p1.reset(new int{73});
  cout << format("p1: {} @ {}\n", *p1, static_cast<void *>(p1.get()));

  shared_ptr<int> p2{new int{42}};
  shared_ptr<int> p3{p2};
  cout << format("p3: {}, cnt {} @ {}\n", *p3, p3.use_count(),
                 static_cast<void *>(p3.get()));
  p2.reset();
  cout << format("p3: {}, cnt {} @ {}\n", *p3, p3.use_count(),
                 static_cast<void *>(p3.get()));

  // weak_ptr, an observer, avoids circular reference
  weak_ptr<int> p4;
  cout << format("p4 expired: {}\n", p4.expired());
  if (auto sp = p4.lock())
    cout << format("p4: {}, cnt: {}\n", *sp, sp.use_count());
  else
    cout << "no shared object\n";

  p4 = p3;
  cout << format("p4 expired: {}\n", p4.expired());
  // lock is preffered over expired since it also conatines the shared_ptr
  if (auto sp = p4.lock())
    cout << format("p4: {}, cnt: {}\n", *sp,
                   sp.use_count());  // 2 due to lock()
  else
    cout << "no shared object\n";

  // share members of a managed object
  struct Animal {
    string name{};
    string sound{};
    Animal(const string &n, const string &s) : name{n}, sound{s} {}
  };
  shared_ptr<Animal> ap{new Animal{"Velociraptor", "Grrrr!"}};
  // the second argument of shared_ptr should be a member of the first argument
  // if not, we need to manually manage the memory of the second argument
  auto np = shared_ptr<string>(ap, &ap->name);
  auto sp = shared_ptr<string>(ap, &ap->sound);
  ap.reset();
  cout << format("name: {}, use_count: {}\n", *np, np.use_count());
  cout << format("sound: {}, use_count: {}\n", *sp, sp.use_count());

  // RNG engines comparison
  histogram<std::random_device>("random_device");
  histogram<std::default_random_engine>("default_random_engine");
  histogram<std::minstd_rand0>("minstd_rand0");
  histogram<std::minstd_rand>("minstd_rand");
  histogram<std::mt19937>("mt19937");
  histogram<std::mt19937_64>("mt19937_64");
  histogram<std::ranlux24_base>("ranlux24_base");
  histogram<std::ranlux48_base>("ranlux48_base");
  histogram<std::ranlux24>("ranlux24");
  histogram<std::ranlux48>("ranlux48");
  histogram<std::knuth_b>("knuth_b");

  // RNG distributions comparison
  // clang-format off
  dist_histogram(std::uniform_int_distribution<int>{0, 9}, "uniform_int_distribution");
  dist_histogram(std::normal_distribution<double>{0.0, 2.0}, "normal_distribution");
  std::initializer_list<double> intervals{0, 5, 10, 30};
  std::initializer_list<double> weights{0.2, 0.3, 0.5};
  dist_histogram(std::piecewise_constant_distribution<double>{begin(intervals), end(intervals), begin(weights)}, "piecewise_constant_distribution");
  std::initializer_list<double> weights2{0, 1, 1, 0};
  dist_histogram(std::piecewise_linear_distribution<double>{begin(intervals), end(intervals), begin(weights2)}, "piecewise_linear_distribution");
  dist_histogram(std::bernoulli_distribution{0.75}, "bernoulli_distribution");
  dist_histogram(std::discrete_distribution<int>{{1, 2, 4, 8}}, "discrete_distribution");
  dist_histogram(std::binomial_distribution<int>{10, 0.3}, "binomial_distribution");
  dist_histogram(std::negative_binomial_distribution<int>{10, 0.8}, "negative_binomial_distribution");
  dist_histogram(std::geometric_distribution<int>{0.4}, "geometric_distribution");
  dist_histogram(std::exponential_distribution<double>{0.4}, "exponential_distribution");
  dist_histogram(std::gamma_distribution<double>{1.5, 1.0}, "gamma_distribution");
  dist_histogram(std::weibull_distribution<double>{1.5, 1.0}, "weibull_distribution");
  dist_histogram(std::extreme_value_distribution<double>{0.0, 1.0}, "extreme_value_distribution");
  dist_histogram(std::lognormal_distribution<double>{0.5, 0.5}, "lognormal_distribution");
  dist_histogram(std::chi_squared_distribution<double>{1.0}, "chi_squared_distribution");
  dist_histogram(std::cauchy_distribution<double>{0.0, 0.1}, "cauchy_distribution");
  dist_histogram(std::fisher_f_distribution<double>{1.0, 1.0}, "fisher_f_distribution");
  dist_histogram(std::student_t_distribution<double>{1.0}, "student_t_distribution");
  // clang-format on
}
