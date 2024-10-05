#include <bits/stdc++.h>

using namespace std;

// POSIX isspace() may not work for all systems, write templated one
template <typename T>
bool isws(const T &c) {
  constexpr const T whitespace[]{" \t\r\n\v\f"};
  for (const T &wsc : whitespace)
    if (c == wsc) return true;
  return false;
}

string delws(const string &s) {
  string outstr{s};
  // std::unique() removes adjacent duplicates
  auto its =
      unique(outstr.begin(), outstr.end(),
             [](const auto &a, const auto &b) { return isws(a) && isws(b); });
  outstr.erase(its, outstr.end());
  outstr.shrink_to_fit();
  return outstr;
}

int main() {
  const string s{"big     bad    \t   wolf"};
  const string s2{delws(s)};
  cout << format("[{}]\n", s);
  cout << format("[{}]\n", s2);
}
