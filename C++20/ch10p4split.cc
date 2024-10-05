#include <bits/stdc++.h>

using namespace std;

template <typename It, typename Oc, typename V, typename Pred>
It split(It it, It end_it, Oc &dest, const V &sep, Pred &f) {
  typename Oc::value_type slice{};
  while (it != end_it) {
    if (f(*it, sep)) {
      dest.push_back(slice);
      slice.clear();
    } else {
      slice.push_back(*it);
    }
    it++;
  }
  if (slice.size()) dest.push_back(slice);
  return it;
};

constexpr auto eq = [](const auto &el, const auto &sep) { return el == sep; };

template <typename It, typename Oc, typename V>
It split(It it, const It end_it, Oc &dest, const V &sep) {
  return split(it, end_it, dest, sep, eq);
}

template <typename In, typename Out, typename V>
Out &strsplit(const In &str, Out &dest, const V &sep) {
  split(str.begin(), str.end(), dest, sep, eq);
  return dest;
}

int main() {
  constexpr char strsep{':'};
  const string str{"sync:x:4:65534:sync:/bin:/bin/sync"};
  vector<string> dest_vs{};
  split(str.begin(), str.end(), dest_vs, strsep, eq);
  for (const auto &e : dest_vs) cout << format("[{}] ", e);
  cout << '\n';

  vector<string> dest_vs2{};
  strsplit(str, dest_vs2, strsep);
  for (const auto &e : dest_vs2) cout << format("[{}] ", e);
  cout << '\n';

  constexpr int intsep{-1};
  vector<int> vi{1, 2, 3, 4, intsep, 5, 6, 7, 8, intsep, 9, 10, 11, 12};
  vector<vector<int>> dest_vi{};
  split(vi.begin(), vi.end(), dest_vi, intsep);
  for (const auto &v : dest_vi) {
    string s;
    for (const auto &e : v) s += format("{}", e);
    cout << format("[{}] ", s);
  }
  cout << '\n';
}
