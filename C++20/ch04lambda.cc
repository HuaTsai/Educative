#include <bits/stdc++.h>

using namespace std;

auto is_div_by(int divisor) {
  return [divisor](int i) { return i % divisor == 0; };
}

template <typename T, typename... Ts>
auto concat(T t, Ts... ts) {
  if constexpr (sizeof...(ts) > 0) {
    return [&](auto... args) { return t(concat(ts...)(args...)); };
  } else {
    return t;
  }
}

template <typename F, typename A, typename B>
auto combine(F binary_func, A a, B b) {
  return [=](auto param) { return binary_func(a(param), b(param)); };
}

int main() {
  // Basic Lambdas

  // Note: according to specification, this is incorrect
  // Therefore, it is recommended to always add parentheses
  auto one = [] { return "one"; };
  // Trailing return type, parentheses required
  auto two = []() -> const char * { return "two"; };

  auto p = [](auto v) { cout << v() << "\n"; };
  p([]() { return "three"; });

  // Anonymous lambda
  cout << [](auto l, auto r) { return l + r; }(1, 2) << "\n";

  int n = 1;
  auto inc = [&n] { n++; };
  for (int i = 0; i < 5; i++) inc();
  cout << n << "\n";

  // Define capture variable, mutable makes n modifiable
  // Since lambda expression defaults to const-qualified
  auto counter = [n = 0]() mutable { return n++; };
  for (int i = 0; i < 5; i++) cout << counter() << " ";
  cout << "\n";

  auto n1 = [=] { return n; };

  // Predicate
  vector<int> vv{1, 7, 4, 9, 4, 8, 12, 10, 20};

  for (int i : {3, 4, 5}) {
    auto pred = is_div_by(i);
    int count = count_if(vv.begin(), vv.end(), pred);
    cout << format("numbers divisible by {}: {}\n", i, count);
  }

  // std::function, polymorphic function container
  auto print_c = [](auto &c) {
    for (auto i : c) cout << format("{} ", i);
    cout << "\n";
  };
  deque<int> d;
  list<int> l;
  vector<int> v;
  auto push_c = [](auto &c) { return [&c](int i) { c.push_back(i); }; };
  // 1. store function/lambda
  const vector<function<void(int)>> consumers = {push_c(d), push_c(l),
                                                 push_c(v)};
  for (auto &c : consumers) {
    for (int i = 0; i < 5; i++) {
      c(i);
    }
  }
  print_c(d);
  print_c(l);
  print_c(v);

  struct A {
    void foo() { cout << "foo\n"; }
    void operator()() { cout << "operator()\n"; }
  };
  // 2. store member function
  const function<void(A &)> f1 = &A::foo;
  A a;
  const function<void(void)> f2 = bind(&A::foo, a);
  f1(a);
  f2();
  // 3. store functor
  const function<void(void)> f3 = a;
  f3();

  // Concatenate lambdas
  auto twice = [](int i) { return i * 2; };
  auto thrice = [](int i) { return i * 3; };
  auto combined = concat(twice, thrice, std::plus<int>{});
  std::cout << format("{}\n", combined(2, 3));

  // Combine predicates with logical conjunction
  cout << "Only words starting with 'a' and ending with 'b' are printed\n";
  auto begin_with = [](const string &s) { return s.find("a") == 0; };
  auto ends_with = [](const string &s) { return s.rfind("b") == s.size() - 1; };
  auto bool_and = [](bool l, bool r) { return l && r; };
  copy_if(istream_iterator<string>(cin), {},
          ostream_iterator<string>(cout, "\n"),
          combine(bool_and, begin_with, ends_with));
  cout << "\n";

  // Multiple lambdas
  auto brace = [](char a, char b) {
    return [a, b](auto x) { return format("{}{}{} ", a, x, b); };
  };
  auto ba = brace('(', ')');
  auto bb = brace('{', '}');
  auto bc = brace('[', ']');
  auto bd = brace('|', '|');
  for (auto i : {1, 2, 3, 4, 5}) {
    for (auto b : {ba, bb, bc, bd}) cout << b(i);
    cout << "\n";
  }

  // Clear cin and stdin
  cin.clear();
  clearerr(stdin);

  // Jump table
  cout << "Jump table: enter A/B/C/D/X\n";
  map<char, void (*)()> jumpmap{// or function<void()>
                                {'A', [] { cout << "func A\n"; }},
                                {'B', [] { cout << "func B\n"; }},
                                {'C', [] { cout << "func C\n"; }},
                                {'D', [] { cout << "func D\n"; }},
                                {'X', [] { cout << "bye!\n"; }}};
  char select{};
  while (select != 'X') {
    std::string r;
    cout << "Please select any of the following character: A/B/C/D/X > ";
    std::getline(cin, r, '\n');
    if (r.size() > 1) {
      cout << "response too long\n";
      continue;
    }
    if (r.size() == 0)
      select = '\0';
    else
      select = toupper(r[0]);
    if (auto it = jumpmap.find(select); it != jumpmap.end())
      it->second();
    else
      cout << "invalid selection\n";
  }
}
