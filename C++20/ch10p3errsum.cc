#include <bits/stdc++.h>

using namespace std;

int main() {
  constexpr size_t vlen{100};
  vector<double> ds(vlen);
  vector<int> is(vlen);

  for (size_t i = 0; i < vlen; ++i) {
    ds[i] = 5.0 * sin(i * 2 * numbers::pi / 100);
  }

  std::transform(ds.begin(), ds.end(), is.begin(),
                 [](double v) { return static_cast<int>(v); });

  for (const auto &v : ds) cout << format("{:-5.2f} ", v);
  cout << "\n\n";
  for (const auto &v : is) cout << format("{:-3d} ", v);
  cout << "\n\n";

  // 1) acc = acc + (*i1) * (*i2)
  // 2) acc = op1(acc, op2(*i1, *i2))
  double errsum =
      inner_product(ds.begin(), ds.end(), is.begin(), 0.0, std::plus<double>(),
                    [](double a, double b) { return pow(a - b, 2); });
  cout << format("error sum: {:.3f}\n\n", errsum);

  cout << "accumulated error without last:\n";
  for (auto it{ds.begin()}; it != ds.end(); ++it) {
    double accumsum =
        inner_product(ds.begin(), it, is.begin(), 0.0, std::plus<double>(),
                      [](double a, double b) { return pow(a - b, 2); });
    cout << format("{:-5.2f} ", accumsum);
  }
  cout << '\n';
}
