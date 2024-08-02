#include <bits/stdc++.h>

using namespace std;

template <typename T>
void pspan(span<T> s) {
  cout << format("number of elements: {}\n", s.size());
  cout << format("size of span: {}\n", s.size_bytes());
  for (auto e : s) cout << format("{} ", e);
  cout << endl;
}

template <typename T>
class Sum {
  T v{};

 public:
  template <typename... Ts>
  Sum(Ts &&...values) : v{(values + ...)} {}
  const T &value() const { return v; }
};

// Template deduction guide
template <typename... Ts>
Sum(Ts &&...ts) -> Sum<std::common_type_t<Ts...>>;

// if constexpr
template <typename T>
auto value_of(T v) {
  if constexpr (is_pointer_v<T>)
    return *v;
  else
    return v;
}

// Challenge
template <typename T>
auto calculate_stats(span<T> arr1, span<T> arr2) {
  // minmax -> algorithm
  auto [min1, max1] = ranges::minmax(arr1);
  auto [min2, max2] = ranges::minmax(arr2);
  // accumulate -> numeric
  auto sum = accumulate(arr1.begin(), arr1.end(), T{0}) +
             accumulate(arr2.begin(), arr2.end(), T{0});
  return make_tuple(sum, min1, max1, min2, max2);
}

int main() {
  // Span: a view over a contiguous sequence of objects
  int carray[] = {1, 2, 3, 4, 5};
  pspan<int>(carray);

  // Structured bindings: must be used with auto
  auto [a, b, c, d, e] = carray;  // or array, tuple, map
  cout << format("{} {} {} {} {}\n", a, b, c, d, e);

  struct S {
    int x;
    double y;
    string z;
  };
  S s{1, 2.7, "hello"};
  auto &[x, y, z] = s;
  cout << format("{} {} {}\n", x, y, z);
  x = 10;
  cout << s.x << endl;

  // Initialize variables within if and switch
  string artist = "Jimi Hendrix";
  if (auto pos{artist.find("Jimi")}; pos != string::npos) {
    cout << format("found\n", pos);
  }

  switch (int i{10}; i) {
    case 10:
      cout << "case 10\n";
      break;
    default:
      cout << "default\n";
  }

  mutex m;
  if (lock_guard<mutex> lg{m}; true) {  // restricts scope of lock_guard
    cout << "locked\n";
  }

  // Template argument deduction
  const char *str = "hello";
  cout << typeid("abc").name() << endl;  // A4_c
  cout << typeid(str).name() << endl;    // PKc
  // C++17: no need to specify types of pair, make_pair is now obsolescent
  cout << typeid(pair(1, 2)).name() << endl;    // St4pairIiiE
  cout << typeid(pair(1L, 2.)).name() << endl;  // St4pairIldE

  // Template deduction guide
  Sum s1{1u, 2.0, 3, 4.0f};  // unsigned, double, int, float
  Sum s2{"abc"s, "def"};     // std::sring, c-string
  auto v1 = s1.value();
  auto v2 = s2.value();
  cout << format("s1 is {} {}\n", typeid(v1).name(), v1);
  cout << format("s2 is {} {}\n", typeid(v2).name(), v2);

  // if constexpr: simplifies compile-time branching
  int x1 = 10;
  int *x1p = &x1;
  cout << format("Value is {}\n", value_of(x1));
  cout << format("Value is {}\n", value_of(x1p));

  // Challenge
  float arr1[] = {1, 2, 3, 4, 5};
  float arr2[] = {6, 7, 8, 9, 10};

  auto [sum, min1, max1, min2, max2] = calculate_stats<float>(arr1, arr2);
  cout << "Input Array 1 : ";
  for (int i = 0; i < sizeof(arr1) / sizeof(float); i++) cout << arr1[i] << " ";
  cout << endl;
  cout << "Input Array 2 : ";
  for (int i = 0; i < sizeof(arr2) / sizeof(float); i++) cout << arr2[i] << " ";
  cout << endl;

  cout << "Sum of Array 1 and Array 2: " << sum << endl;
  cout << "Min in Array 1: " << min1 << endl;
  cout << "Max in Array 1: " << max1 << endl;
  cout << "Min in Array 2: " << min2 << endl;
  cout << "Max in Array 2: " << max2 << endl;
}
