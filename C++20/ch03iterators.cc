#include <bits/stdc++.h>

using namespace std;

// random_access_iterator is a concept
template <typename T>
  requires random_access_iterator<typename T::iterator>
void printc(const T &c) {
  for (auto e : c) cout << format("{} ", e);
  cout << endl;
  for (auto i = 0; i < c.size(); ++i)
    cout << format("element {}: {}\n", i, c[i]);
}

template <typename T>
class Seq {
 public:
  Seq(T start, T end) : start_{start}, end_{end} {}

  // Custom iterator, usually defined as a nested class
  // Range-based for needs:
  // 1. begin() and end() member functions that return iterator
  // 2. the iterator should support operator*, operator++, operator!=
  //
  // STL algorithm: e.g, std::minmax_element, needs: operator==
  class iterator {
   public:
    // six type traits of iterator
    using iterator_concept = std::forward_iterator_tag;
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::remove_cv_t<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    explicit iterator(T position = 0) : value_{position} {}
    T operator*() const { return value_; }
    iterator &operator++() {
      ++value_;
      return *this;
    }

    // Postfix increment, so that ranges::minmax_element works
    // Also, static_assert(ranges::forward_range<Seq<int>>); passes
    iterator operator++(T) {
      iterator temp = *this;
      ++*this;
      return temp;
    }

    bool operator!=(const iterator &other) const {
      return value_ != other.value_;
    }
    bool operator==(const iterator &other) const {
      return value_ == other.value_;
    }

   private:
    T value_{};
  };

  iterator begin() const { return iterator{start_}; }
  iterator end() const { return iterator{end_}; }

 private:
  T start_{};
  T end_{};
};

void printcc(const auto &v, const string_view s = "") {
  if (s.size()) cout << format("{}: ", s);
  for (auto e : v) cout << format("{} ", e);
  cout << "\n";
}

// Generator iterator
class fib_generator {
  using fib_t = unsigned long;

 public:
  using iterator_concept = std::forward_iterator_tag;
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::remove_cv_t<fib_t>;
  using difference_type = std::ptrdiff_t;
  using pointer = const fib_t *;
  using reference = const fib_t &;

  explicit fib_generator(fib_t stop = 0) : stop_{stop} {}

  fib_t operator*() const { return b_; }

  constexpr fib_generator &operator++() {
    do_fib();
    ++count_;
    return *this;
  }

  fib_generator operator++(int) {
    auto temp{*this};
    ++*this;
    return temp;
  }

  bool operator!=(const fib_generator &o) const { return count_ != o.count_; }
  bool operator==(const fib_generator &o) const { return count_ == o.count_; }

  const fib_generator &begin() const { return *this; }
  const fib_generator end() const {
    auto sentinel = fib_generator();
    sentinel.count_ = stop_;
    return sentinel;
  }

  fib_t size() { return stop_; }

 private:
  fib_t stop_{};
  fib_t count_{0};
  fib_t a_{0};
  fib_t b_{1};

  constexpr void do_fib() {
    a_ += b_;
    swap(a_, b_);
  }
};

// Sentinel for C-string
using sentinel_t = const char;
constexpr sentinel_t nullchar = '\0';

// Define sentinel value, end(), and operator!=
class cstr_it {
 public:
  explicit cstr_it(const char *str) : s{str} {}
  char operator*() const { return *s; }
  cstr_it &operator++() {
    ++s;
    return *this;
  }
  bool operator!=(sentinel_t) const { return s != nullptr && *s != nullchar; }
  cstr_it begin() const { return *this; }
  sentinel_t end() const { return nullchar; }

 private:
  const char *s{};
};

template <typename T>
class zip_iterator {
  using val_t = typename T::value_type;
  using ret_t = std::pair<val_t, val_t>;
  using it_t = typename T::iterator;

 public:
  using iterator_concept = std::forward_iterator_tag;
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::pair<val_t, val_t>;
  using difference_type = long int;
  using pointer = const val_t *;
  using reference = const val_t &;

  zip_iterator(T &a, T &b)
      : ita_{a.begin()},
        itb_{b.begin()},
        ita_begin_{ita_},
        itb_begin_{itb_},
        ita_end_{a.end()},
        itb_end_(b.end()) {}

  zip_iterator &operator++() {
    ++ita_;
    ++itb_;
    return *this;
  }

  bool operator==(const zip_iterator &o) const {
    return ita_ == o.ita_ && itb_ == o.itb_;
  }

  bool operator!=(const zip_iterator &o) const { return !operator==(o); }

  ret_t operator*() const { return {*ita_, *itb_}; }

  zip_iterator begin() const { return zip_iterator{ita_begin_, itb_begin_}; }
  zip_iterator end() const { return zip_iterator{ita_end_, itb_end_}; }

 private:
  zip_iterator(it_t ita, it_t itb) : ita_{ita}, itb_{itb} {}
  it_t ita_{};
  it_t itb_{};
  it_t ita_begin_{};
  it_t ita_end_{};
  it_t itb_begin_{};
  it_t itb_end_{};
};

template <typename T>
class Container {
 public:
  Container(initializer_list<T> l) : n_elements_{l.size()} {
    // Constructs an array of the given dynamic size. The array elements are
    // value-initialized. This overload participates in overload resolution only
    // if T is an array of unknown bound.
    c_ = std::make_unique<T[]>(n_elements_);
    size_t index{0};
    for (T e : l) {
      c_[index++] = e;
    }
  }

  Container(size_t sz) : n_elements_{sz} {
    c_ = std::make_unique<T[]>(n_elements_);
  }

  size_t size() const { return n_elements_; }

  const T &operator[](const size_t index) const { return c_[index]; }

  T &at(const size_t index) const {
    if (index > n_elements_ - 1) {
      throw std::out_of_range("Container::at(): index out of range");
    }
    return c_[index];
  }

  bool empty() const { return n_elements_ == 0; }

  class iterator {
   public:
    using iterator_concept = std::contiguous_iterator_tag;
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    iterator(T *ptr = nullptr) : ptr_{ptr} {}

    iterator &operator++() {
      ++ptr_;
      return *this;
    }

    iterator operator++(int) {
      auto ret = *this;
      ++ptr_;
      return ret;
    }

    iterator &operator--() {
      --ptr_;
      return *this;
    }

    iterator operator--(int) {
      auto ret = *this;
      --ptr_;
      return ret;
    }

    T &operator[](const size_t index) const { return *(ptr_ + index); }

    // default comparison operator (C++20)
    const auto operator<=>(const iterator &o) const { return ptr_ <=> o.ptr_; }

    const bool operator==(const iterator &o) const { return ptr_ == o.ptr_; }

    T &operator*() const { return *ptr_; }

    T *operator->() const { return ptr_; }

    iterator operator+(const size_t n) const { return iterator(ptr_ + n); }

    // non-member operator (n + it)
    friend const iterator operator+(const size_t n, const iterator &o) {
      return iterator(o.ptr_ + n);
    }

    const iterator operator-(const size_t n) { return iterator(ptr_ - n); }

    const size_t operator-(const iterator &o) { return ptr_ - o.ptr_; }

    iterator &operator+=(const size_t n) {
      ptr_ += n;
      return *this;
    }

    iterator &operator-=(const size_t n) {
      ptr_ -= n;
      return *this;
    }

   private:
    T *ptr_;
  };

  iterator begin() const { return iterator(c_.get()); }

  iterator end() const { return iterator(c_.get() + n_elements_); }

 private:
  std::unique_ptr<T[]> c_{};
  size_t n_elements_{};
};

void unit_tests(Container<string> &x) {
  puts("\nunit tests =============================");

  auto a = x.begin();
  auto b = x.begin() + 5;
  auto n = b - a;
  bool r{};

  cout << format("*a: {}\n", *a);
  cout << format("*b: {}\n", *b);
  cout << format("n: {}\n", n);

  r = (a += n) == b;
  cout << format("{}: (a += n) == b\n", r);

  a = x.begin();
  r = std::addressof(a += n) == std::addressof(a);
  cout << format("{}: std::addressof(a += n) == std::addressof(a)\n", r);

  a = x.begin();
  auto it1 = (a + n);
  auto it2 = (a += n);
  r = it1 == it2;
  cout << format("{}: (a + n) == (a += n)\n", r);

  a = x.begin();
  r = (a + n) == (n + a);
  cout << format("{}: (a + n) == (n + a)\n", r);

  int i{1};
  int j{2};
  r = a + (i + j) == (a + i) + j;
  cout << format("{}: a + (i + j) == (a + i) + j\n", r);

  r = a + 0 == a;
  cout << format("{}: a + 0 == a\n", r);

  b = x.begin() + n;
  r = --b == (a + (n - 1));
  cout << format("{}: --b == (a + (n - 1))\n", r);

  b = x.begin() + n;
  it1 = b += -n;
  b = x.begin() + n;
  it2 = b -= n;
  r = it1 == a && it2 == a;
  cout << format("{}: (b += -n) == a && (b -= n) == a\n", r);

  b = x.begin() + n;
  r = std::addressof(b -= n) == std::addressof(b);
  cout << format("{}: std::addressof(b -= n) == std::addressof(b)\n", r);

  b = x.begin() + n;
  it1 = b - n;
  b = x.begin() + n;
  it2 = b -= n;
  r = it1 == a && it2 == a;
  cout << format("{}: (b - n) == (b -= n)\n", r);

  b = x.begin() + n;
  r = a[n] == *b;
  cout << format("{}: a[n] == *b\n", r);

  r = (a <= b);
  cout << format("{}: (a <= b)\n", r);
}

int main() {
  vector<int> c{1, 2, 3, 4, 5};
  printc(c);  // CE if c is list<int>

  Seq<int> r{100, 110};
  for (auto e : r) cout << format("{} ", e);
  cout << endl;

  // Iterators compatible with STL iterator traits
  auto [min_it, max_it] =
      minmax_element(r.begin(), r.end());  // needs operator==
  cout << format("min: {}, max: {}\n", *min_it, *max_it);

  ranges::minmax_element(r);
  static_assert(ranges::forward_range<Seq<int>>);

  // Iterator adaptors
  deque<int> d1{1, 2, 3, 4, 5};
  deque<int> d2(d1.size());
  copy(d1.begin(), d1.end(), d2.begin());
  printcc(d1);
  printcc(d2, "d2 after copy");
  copy(d1.begin(), d1.end(), back_inserter(d2));
  printcc(d2, "d2 after back_inserter");
  deque<int> d3{47, 73, 114, 138, 54};
  copy(d3.begin(), d3.end(), front_inserter(d2));
  printcc(d2, "d2 after front_inserter");
  auto it2{d2.begin() + 2};
  copy(d1.begin(), d1.end(), inserter(d2, it2));
  printcc(d2, "d2 after middle insert");
  cout << "ostream_iterator: ";
  copy(d1.begin(), d1.end(), ostream_iterator<int>(cout, " "));
  cout << "\n";
  cout << "istream_iterator: ";
  vector<string> vs{};
  // istream_iterator<string>() -> end iterator, sentinel
  copy(istream_iterator<string>(cin), istream_iterator<string>(),
       back_inserter(vs));
  printcc(vs, "vs");
  cout << "reverse_iterator: ";
  for (auto it = d1.rbegin(); it != d1.rend(); ++it) cout << format("{} ", *it);
  cout << "\n";

  // Generator iterator
  printcc(fib_generator(10));
  fib_generator fib(10);
  auto xx =
      ranges::views::transform(fib, [](unsigned long x) { return x * x; });
  printcc(xx, "squared");

  // C-string iterator with sentinel
  const char carray[]{"array"};
  for (auto c : cstr_it(carray)) cout << format("{:02x} ", c);
  cout << "\n";

  const char *cstr{"c-string"};
  for (auto c : cstr_it(cstr)) cout << format("{:02x} ", c);
  cout << "\n";

  // Zip iterator
  vector<string> va{"Bob", "John", "Joni"};
  vector<string> vb{"Dylan", "Williams", "Mitchell"};

  cout << "va: ";
  for (auto e : va) cout << format("{} ", e);
  cout << '\n';

  cout << "vb: ";
  for (auto e : vb) cout << format("{} ", e);
  cout << '\n';

  cout << "zipped: ";
  for (auto [a, b] : zip_iterator(va, vb)) {
    cout << format("[{}, {}] ", a, b);
  }
  cout << '\n';

  map<string, string> name_map{};

  for (auto [a, b] : zip_iterator(va, vb)) {
    name_map.try_emplace(a, b);
  }

  cout << "name_map: ";
  for (auto [a, b] : name_map) {
    cout << format("[{}, {}] ", a, b);
  }
  cout << '\n';

  // Test for random access iterator
  Container<string> x{"one", "two",   "three", "four", "five",
                      "six", "seven", "eight", "nine", "ten"};
  cout << format("Container x size: {}\n", x.size());

  puts("Container x:");
  for (auto e : x) {
    cout << format("{} ", e);
  }
  cout << '\n';

  puts("direct access elements:");
  cout << format("element at(5): {}\n", x.at(5));
  cout << format("element [5]: {}\n", x[5]);
  cout << format("element begin + 5: {}\n", *(x.begin() + 5));
  cout << format("element 5 + begin: {}\n", *(5 + x.begin()));
  cout << format("element begin += 5: {}\n", *(x.begin() += 5));

  puts("views pipe reverse:");
  auto result = x | views::reverse;
  for (auto v : result) cout << format("{} ", v);
  cout << '\n';

  Container<string> y(x.size());
  cout << format("Container y size: {}\n", y.size());
  for (auto e : y) {
    cout << format("[{}] ", e);
  }
  cout << '\n';

  unit_tests(x);
}
