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

int main() {
  vector<int> c{1, 2, 3, 4, 5};
  printc(c);  // CE if c is list<int>

  Seq<int> r{100, 110};
  for (auto e : r) cout << format("{} ", e);
  cout << endl;

  // Iterators compatible with STL iterator traits
  auto [min_it, max_it] = minmax_element(r.begin(), r.end());  // needs operator==
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
  copy(istream_iterator<string>(cin), istream_iterator<string>(), back_inserter(vs));
  printcc(vs, "vs");
  cout << "reverse_iterator: ";
  for (auto it = d1.rbegin(); it != d1.rend(); ++it) cout << format("{} ", *it);
  cout << "\n";
}
