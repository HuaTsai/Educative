#include <bits/stdc++.h>

using namespace std;

void printc(auto &c, string_view s = "") {
  if (s.size()) cout << format("{}: ", s);
  for (auto e : c) cout << format("[{}] ", e);
  cout << '\n';
}

void printcalign(auto &c, string_view s = "") {
  if (s.size()) cout << format("{}: ", s);
  for (auto e : c) cout << format("{:>5} ", e);
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

struct Things {
  string s_;
  int i_;
  string str() const { return format("({}, {})", s_, i_); }
};

struct City {
  string name;
  int population;
  bool operator==(const City &c) const { return name == c.name; }
  string str() const { return format("[{}, {}]", name, population); }
};

vector<int> merge_sort(const vector<int> &v1, const vector<int> &v2) {
  if (!v1.size()) return v2;
  if (!v2.size()) return v1;
  auto v11 = merge_sort(vector<int>(v1.begin(), v1.begin() + v1.size() / 2),
                        vector<int>(v1.begin() + v1.size() / 2, v1.end()));
  auto v22 = merge_sort(vector<int>(v2.begin(), v2.begin() + v2.size() / 2),
                        vector<int>(v2.begin() + v2.size() / 2, v2.end()));
  vector<int> result;
  ranges::merge(v11, v22, back_inserter(result));
  return result;
}

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
  vector<Things> vt{{"button", 40},
                    {"hamburger", 20},
                    {"blog", 1000},
                    {"page", 100},
                    {"science", 60}};
  ranges::sort(vt,
               [](const Things &a, const Things &b) { return a.i_ < b.i_; });
  for (auto &t : vt) cout << t.str() << " ";
  cout << "\n";

  // Transform
  vector<int> vv1, vv2;
  ranges::transform(v, back_inserter(vv1), [](int x) { return x * x; });
  printc(vv1, "squared");

  auto view1 = views::transform(v, [](int x) { return x * x; });
  printc(view1, "squared view");

  // Find
  const vector<City> c{{"London", 9425622},
                       {"Berlin", 3566791},
                       {"Tokyo", 37435191},
                       {"Cairo", 20485965}};
  // Need implement operator==
  auto it = ranges::find(c, City{"Berlin"});
  if (it != c.end())
    cout << format("Found: {}\n", it->str());
  else
    cout << "Not found\n";

  // Find if
  auto it2 = ranges::find_if(
      c, [](const City &c) { return c.population > 20'000'000; });
  if (it2 != c.end())
    cout << format("Found: {}\n", it2->str());
  else
    cout << "Not found\n";

  // views::filter, view adapter of all matchings
  auto view2 =
      views::filter(c, [](const City &c) { return c.population > 20'000'000; });
  // or use c | views::filter([](Pred &&pred))
  for (const auto &city : view2) cout << format("{} ", city.str());
  cout << "\n";

  // Clamp
  list<int> il = {0, -12, 2001, 4, 5, -14, 100, 200, 30000};
  printcalign(il, "original il");
  ranges::transform(il, il.begin(),
                    [](auto e) { return ranges::clamp(e, 0, 500); });
  printcalign(il, " clamped il");

  // Sample
  constexpr size_t n_data{200'000};
  constexpr size_t n_samples{500};
  constexpr int mean{0};
  constexpr int dev{3};
  random_device rd;
  mt19937 gen{rd()};
  normal_distribution<> dist{mean, dev};
  array<int, n_data> data;
  for (auto &e : data) e = lround(dist(gen));
  array<int, n_samples> samples;
  ranges::sample(data, samples.begin(), n_samples, gen);
  map<int, size_t> hist;
  for (int i : samples) ++hist[i];
  constexpr size_t scale{3};
  cout << format("{:>3} {:>5} {:<}/{}\n", "n", "count", "graph", scale);
  for (const auto &[value, count] : hist) {
    cout << format("{:>3} ({:>3}) {}\n", value, count,
                   string(count / scale, '*'));
  }

  // Next permutation
  vector<int> vi{1, 2, 3, 4, 5, 1, 2, 3, 4, 5};
  ranges::sort(vi);
  int count{};
  do {
    ++count;
  } while (ranges::next_permutation(vi).found);
  cout << format("number of permuatations: {}\n", count);

  // Next permutation with projection
  vector<pair<int, string>> vp{{1, "one"}, {2, "two"}, {3, "three"}};
  do {
    for (const auto &[i, s] : vp) cout << format("[{}, {}] ", i, s);
    cout << '\n';
  } while (ranges::next_permutation(vp, {}, [](const auto &p) {
             return p.first;
           }).found);

  // Merge sorted containers
  vector<string> vs1{"dog", "cat", "velociraptor"};
  vector<string> vs2{"kirk", "sulu", "spock"};
  vector<string> dest{};
  printc(vs1, "vs1");
  printc(vs2, "vs2");
  ranges::merge(vs1, vs2, back_inserter(dest));
  printc(dest, "merged");

  // Merge sort
  auto v33 = merge_sort(vector<int>{3, 5, 2, 8}, vector<int>{9, 1, 7, 4});
  printc(v33);
}
