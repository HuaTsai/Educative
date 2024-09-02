#include <bits/stdc++.h>

using namespace std;

void printc(auto &c, string_view s = "") {
  if (s.size()) cout << format("{}: ", s);
  for (auto e : c) cout << format("[{}] ", e);
  cout << '\n';
}

template <typename I>
ostream &join(I it, I end_it, ostream &o, string_view sep = " ") {
  if (it != end_it) o << *it++;
  while (it != end_it) o << sep << *it++;
  return o;
}

template <typename I>
string join(I it, I end_it, string_view sep = " ") {
  ostringstream oss;
  join(it, end_it, oss, sep);
  return oss.str();
}

string join(const auto &c, string_view sep = " ") {
  return join(c.begin(), c.end(), sep);
}

void printsort(auto &c) {
  if (!ranges::is_sorted(c)) cout << "un";
  cout << "sorted: ";
  for (auto &e : c) cout << e << " ";
  cout << "\n";
}

void randomize(auto &c) {
  static random_device rd;
  static default_random_engine re{rd()};
  ranges::shuffle(c, re);
}

struct things {
  string s_;
  int i_;
  string str() const { return format("({}, {})", s_, i_); }
};

int main() {
  // Copy
  vector<string> v1{"alpha", "beta", "gamma", "delta", "epsilon"};
  printc(v1, "v1");
  vector<string> v2;
  ranges::copy(v1, back_inserter(v2));
  printc(v2, "v2");
  vector<string> v3;
  ranges::copy_n(v1.begin(), 3, back_inserter(v3));
  printc(v3, "v3");
  vector<string> v4;
  ranges::copy_if(v1, back_inserter(v4),
                  [](const string &s) { return s.size() > 4; });
  printc(v4, "v4");
  ostream_iterator<string> out_it{cout, " "};
  ranges::copy(v1, out_it);
  cout << '\n';

  // Move
  ranges::move(v1, v2.begin());
  printc(v1, "after move: v1");
  printc(v2, "after move: v2");

  // Join elements
  for (auto c : views::join(v2)) cout << c;  // no separator
  cout << '\n';
  join(v2.begin(), v2.end(), cout, ", ") << '\n';    // direct output
  cout << join(v2.begin(), v2.end(), ", ") << '\n';  // return string
  cout << join(v2, ", ") << '\n';  // return string, more convenient
  list<double> constants{std::numbers::pi, std::numbers::e,
                         std::numbers::sqrt2};
  cout << join(constants, ", ") << '\n';
  cout << join(views::join(v2), ":")
       << '\n';  // also work with range::view object

  // Sort
  cout << "Sort:\n";
  vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  randomize(v);
  printsort(v);
  ranges::sort(v);
  printsort(v);

  // Partial sort
  cout << "Partial sort:\n";
  randomize(v);
  printsort(v);
  auto middle = v.begin() + v.size() / 2;
  ranges::partial_sort(v.begin(), middle, v.end());
  printsort(v);

  // Partition
  cout << "Partition:\n";
  randomize(v);
  printsort(v);
  ranges::partition(v, [](int i) { return i > 5; });
  printsort(v);

  // Custom sort
  vector<things> vt{{"button", 40},
                    {"hamburger", 20},
                    {"blog", 1000},
                    {"page", 100},
                    {"science", 60}};
  ranges::sort(vt,
               [](const things &a, const things &b) { return a.i_ < b.i_; });
  for (auto &t : vt) cout << t.str() << " ";
  cout << "\n";

  // Transform
  vector<int> vv1, vv2;
  ranges::transform(v, back_inserter(vv1), [](int x) { return x * x; });
  printc(vv1, "squared");

  auto view1 = views::transform(v, [](int x) { return x * x; });
  printc(view1, "squared view");
}
