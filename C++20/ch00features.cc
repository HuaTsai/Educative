#include <bits/stdc++.h>
import myadd;

namespace ranges = std::ranges;
namespace views = std::ranges::views;

using namespace std;

struct Frac {
  long n, d;
};

template <>
struct std::formatter<Frac> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Frac &f, FormatContext &ctx) const {
    return format_to(ctx.out(), "{0:d}/{1:d}", f.n, f.d);
  }
};

template <typename... Args>
void print(const string_view fmt_str, Args &&...args) {
  auto fmt_args{make_format_args(args...)};
  string outstr{vformat(fmt_str, fmt_args)};
  fputs(outstr.c_str(), stdout);
}

constexpr auto user_vector() {
  std::vector<int> v{1, 2, 3, 4, 5};
  return accumulate(v.begin(), v.end(), 0);
}

// Feature 6: requires
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template <typename T>
  requires Numeric<T>
T arg42(const T &arg) {
  return arg + 42;
}

// Other methods
// 1. template <Numeric T>
// 2. T arg42(const T &arg) requires Numeric<T> {}
// 3. auto arg42(Numeric auto & arg) {} (w/o template)

int main() {
  // Feature 1: std::format
  Frac f{5, 3};
  print("f = {}\n", f);

  // Feature 2: constexpr
  constexpr int sum = user_vector();
  std::cout << "sum = " << sum << std::endl;

  // Feature 3: std::cmp_less
  int x = -3;
  unsigned int y = 3;
  if (std::cmp_less(x, y))
    std::cout << "x < y" << std::endl;
  else
    std::cout << "x >= y" << std::endl;

  // Feature 4: three-way comparison / spaceship operator
  // convert < to conventional comparison: (a <=> b) < 0
  struct Num {
    int n;
    constexpr Num(int n) : n(n) {}
    // old method
    // constexpr bool operator<(const Num &rhs) const { return n < rhs.n; }
    // constexpr bool operator>(const Num &rhs) const { return n > rhs.n; }
    // ...

    // new method
    auto operator<=>(const Num &) const = default;
  };

  struct Frac2 {
    long n, d;
    constexpr Frac2(long n, long d) : n(n), d(d) {}
    constexpr double dbl() const { return static_cast<double>(n) / d; }
    constexpr auto operator<=>(const Frac2 &rhs) const {
      return dbl() <=> rhs.dbl();
    }
    // Note: if you define <=>, you must define == as well
    constexpr auto operator==(const Frac2 &rhs) const {
      return dbl() == rhs.dbl();
    }
  };
  static_assert(Num(1) < Num(2));
  static_assert(Num(2) == Num(2));
  static_assert(Frac2(1, 2) < Frac2(2, 3));
  static_assert(Frac2(1, 2) == Frac2(2, 4));

  // Feature 5: version macro
#ifdef __cpp_lib_three_way_comparison
  std::cout << "Three-way comparison supported: "
            << __cpp_lib_three_way_comparison << std::endl;
#endif
#if __has_include(<format>)
  std::cout << "Header <format> found" << std::endl;
#endif

  // Feature 6: requires
  std::cout << arg42(1) << std::endl;
  std::cout << arg42(1.0) << std::endl;
  // arg42("hello");  // CE

  // Feature 7: modules (partially supported in g++)
  // g++ -std=c++20 -fmodules-ts -c myadd.cc
  // g++ -std=c++20 -fmodules-ts 00features.cc myadd.o
  std::cout << add(1, 2) << std::endl;

  // Feature 8: Views
  const vector<int> nums{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (auto v : ranges::take_view(nums, 5)) cout << v << " ";
  std::cout << std::endl;
  for (auto v : nums | views::take(5) | views::reverse) cout << v << " ";
  std::cout << std::endl;
  for (auto v : nums | views::transform([](int i) { return i * i; }))
    cout << v << " ";
  std::cout << std::endl;
  for (auto v : nums | views::filter([](int i) { return i % 2 == 0; }))
    cout << v << " ";
  std::cout << std::endl;
  auto rnums = views::iota(1, 10);  // old iota needs to write more code
  for (auto v : rnums) cout << v << " ";
  std::cout << std::endl;
  auto rnums2 = views::iota(1) | views::take(10);
  for (auto v : rnums2) cout << v << " ";
  std::cout << std::endl;

  // More tools
  std::vector<int> nums2{1, 4, 2, 3, 5, 100, 100, 100, 100, 100};
  ranges::sort(nums2 | views::reverse | views::drop(5));
  for (auto v : nums2) cout << v << " ";
  std::cout << std::endl;
}
