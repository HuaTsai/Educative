#include <bits/stdc++.h>

using namespace std;

// Input : first ... pivot - 1  pivot ... last
// Result: <part_no> <part_yes> <part_yes> <part_no>
// Output:    it1                   it2
template <typename It, typename Pred>
pair<It, It> gather(It first, It last, It pivot, Pred pred) {
  return {stable_partition(first, pivot, not_fn(pred)),
          stable_partition(pivot, last, pred)};
}

constexpr auto midit = [](auto &v) {
  return v.begin() + (v.end() - v.begin()) / 2;
};

constexpr auto is_even = [](auto i) { return i % 2 == 0; };

constexpr auto is_even_char = [](auto c) {
  if (c >= '0' && c <= '9')
    return (c - '0') % 2 == 0;
  else
    return false;
};

int main() {
  vector<int> vint{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  string jenny{"867-5309"};

  auto gathered_even =
      gather(vint.begin(), vint.end(), midit(vint), is_even);
  for (const auto &el : vint) cout << el;
  cout << '\n';
  auto &[it1, it2] = gathered_even;
  for (auto it{it1}; it < it2; ++it) cout << *it;
  cout << '\n';

  gather(vint.begin(), vint.end(), vint.begin(), is_even);
  for (const auto &el : vint) cout << el;
  cout << '\n';

  gather(vint.begin(), vint.end(), vint.end(), is_even);
  for (const auto &el : vint) cout << el;
  cout << '\n';

  gather(jenny.begin(), jenny.end(), jenny.end(), is_even_char);
  for (const auto &el : jenny) cout << el;
  cout << '\n';
}
