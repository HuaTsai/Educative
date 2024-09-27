#include <bits/stdc++.h>

using namespace std;
using namespace std::string_literals;

template <typename T>
struct Point {
  T x, y;
};

template <typename T>
std::ostream &operator<<(std::ostream &os, const Point<T> &p) {
  return os << "(" << p.x << ", " << p.y << ")";
}

// formatter for Point<T>
template <typename T>
struct std::formatter<Point<T>> : std::formatter<unsigned> {
  template <typename Context>
  auto format(const Point<T> &p, Context &ctx) const {
    return format_to(ctx.out(), "({}, {})", p.x, p.y);
  }
};

struct City {
  string name;
  unsigned int population;
  double latitude;
  double longitude;
};

istream &operator>>(istream &in, City &c) {
  in >> ws;  // skip whitespace
  getline(in, c.name);
  in >> c.population >> c.latitude >> c.longitude;
  return in;
}

// Windows: skip BOM
void skip_bom(auto &fs) {
  const unsigned char boms[]{0xef, 0xbb, 0xbf};
  bool have_bom{true};
  for (const auto &c : boms) {
    if ((unsigned char)fs.get() != c) {
      have_bom = false;
    }
  }
  if (!have_bom) fs.seekg(0);
  return;
}

// Custom char_traits
constexpr char char_lower(const char &c) { return tolower(c); }

class ci_traits : public std::char_traits<char> {
 public:
  static constexpr bool lt(char_type a, char_type b) noexcept {
    return char_lower(a) < char_lower(b);
  }

  static constexpr bool eq(char_type a, char_type b) noexcept {
    return char_lower(a) == char_lower(b);
  }

  static constexpr int compare(const char_type *s1, const char_type *s2,
                               size_t count) {
    for (size_t i{0}; i < count; ++i) {
      auto diff{char_lower(s1[i]) <=> char_lower(s2[i])};
      if (diff > 0) return 1;
      if (diff < 0) return -1;
    }
    return 0;
  }

  static constexpr const char_type *find(const char_type *p, size_t count,
                                         const char_type &ch) {
    const char_type find_c{char_lower(ch)};
    for (size_t i{0}; i < count; ++i) {
      if (find_c == char_lower(p[i])) return p + i;
    }
    return nullptr;
  }
};

using ci_string = basic_string<char, ci_traits>;

ostream &operator<<(ostream &os, const ci_string &ci) {
  return os << ci.c_str();
}

class lc_traits : public std::char_traits<char> {
 public:
  static constexpr void assign(char_type &r, const char_type &a) noexcept {
    r = char_lower(a);
  }

  static constexpr char_type *assign(char_type *p, std::size_t count,
                                     char_type a) {
    for (size_t i{}; i < count; ++i) p[i] = char_lower(a);
    return p;
  }

  static constexpr char_type *copy(char_type *dest, const char_type *src,
                                   size_t count) {
    for (size_t i{0}; i < count; ++i) {
      dest[i] = char_lower(src[i]);
    }
    return dest;
  }
};

using lc_string = basic_string<char, lc_traits>;

template <typename It>
void get_links(It it) {
  for (It end_it{}; it != end_it;) {
    const string link{*it++};
    if (it == end_it) break;
    const string desc{*it++};
    cout << format("{:.<24} {}\n", desc, link);
  }
}

int main() {
  // string_view:
  // reference to a contiguous sequence of characters
  // modify methods are not available
  char text[] = "hello";
  string_view sv{text};
  cout << format("text: {} {}\n", (void *)text, text);
  cout << format("  sv: {} {}\n", (void *)sv.data(), sv);
  text[0] = 'J';
  cout << format("text: {} {}\n", (void *)text, text);
  cout << format("  sv: {} {}\n", (void *)sv.data(), sv);

  // concatenate strings
  // benchmark: append > operator+ > format >> ostringstream
  // The format() function or the string object's + operator works well for most
  // circumstances. The append() version may be the best choice if every bit of
  // speed matters. The ostringstream class works well when we need its unique
  // features and performance is not an issue.

  // transform
  auto rot13 = [](char c) {
    if (isupper(c)) {
      return static_cast<char>((c - 'A' + 13) % 26 + 'A');
    } else if (islower(c)) {
      return static_cast<char>((c - 'a' + 13) % 26 + 'a');
    } else {
      return c;
    }
  };
  string s = "Hello, World!";
  cout << format("original string: {}\n", s);
  ranges::transform(s, s.begin(), rot13);
  cout << format("after rot13: {}\n", s);
  ranges::transform(s, s.begin(), rot13);
  cout << format("rot13 again: {}\n", s);

  // format
  cout << format("Hex: {:x} Octal: {:o} Decimal: {:d}\n", 42, 42, 42);
  cout << format("{1}, {0}!\n", "World", "Hello");  // may be useful for i18n
  cout << format("{{:.5}}: {:.5}\n", M_PI);
  cout << format("[{{:10}}]: [{:10}] \n", 47);
  cout << format("[{{:<10}}]: [{:<10}]\n", 47);
  cout << format("[{{:>10}}]: [{:>10}]\n", 47);
  cout << format("[{{:*<10}}]: [{:*<10}]\n", 47);
  cout << format("[{{:*>10}}]: [{:*>10}]\n", 47);
  cout << format("{:>8} [{{:04x}}]: [{:04x}]\n", "Hex", 47);
  cout << format("{:>8} [{{:4o}}]: [{:4o}]\n", "Octal", 47);
  cout << format("{:>8} [{{:4d}}]: [{:4d}]\n", "Decimal", 47);

  // <print> will be available in C++ 23
  // print("Hello, {}\n", "World!");

  // old: define ostream
  cout << Point<int>{1, 2} << "\n";
  cout << Point<double>{1, 2} << "\n";

  // new: define formatter
  cout << format("Point<int>: {}\n", Point<int>{1, 2});
  cout << format("Point<double>: {}\n", Point<double>{1, 2});

  // trim whitespaces
  auto trim = [](const string &s) -> string {
    constexpr const char *whitespace = " \t\n\r\f\v";
    const auto first{s.find_first_not_of(whitespace)};
    if (first == string::npos) return {};
    const auto last{s.find_last_not_of(whitespace)};
    return s.substr(first, last - first + 1);
  };

  string s2{" \t  ten-thumbed input   \t   \n \t "};
  cout << format("[{}]\n", s2);
  cout << format("[{}]\n", trim(s2));

  // user input
  string word;
  while (cin >> word) {
    cout << format("[{}] ", word);
  }
  cout << "\n";

  // clear cin and EOF
  cin.clear();
  clearerr(stdin);

  auto prompt = [](const string_view s, const string_view s2 = "") -> bool {
    if (s2.size())
      cout << format("{} ({}): ", s, s2);
    else
      cout << format("{}: ", s);
    cout.flush();
    return true;
  };

  constexpr size_t MAXLINE{1024 * 10};
  char line[MAXLINE]{};
  const char *p1{"Words here"};
  prompt(p1);
  // (char *s, streamsize n, char delim = '\n')
  cin.getline(line, MAXLINE, '\n');
  cout << line << "\n";
  prompt("More words here", "p1a");
  string line_str;
  // (istream &is, string &s, char delim = '\n')
  getline(cin, line_str, '\n');
  cout << line_str << "\n";

  double a, b;
  const char *p2{"Enter two numbers"};
  for (prompt(p2); !(cin >> a >> b); prompt(p2)) {
    cout << "not numeric\n";
    // clear cin and buffer
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
  cout << format("a: {} b: {}\n", a, b);

  // stringstream
  line_str.clear();
  const char *p3{"Comma-separated words"};
  prompt(p3);
  while (line_str.empty()) getline(cin, line_str);
  stringstream ss(line_str);
  while (getline(ss, word, ',')) {
    if (word.empty()) continue;
    cout << format("word: [{}]\n", trim(word));
  }

  // word count
  using it_t = istream_iterator<string>;
  ifstream ifs{"input.txt"};
  cout << format("word count: {}\n", distance(it_t{ifs}, it_t{}));

  // complext data structure
  vector<City> cities;
  ifstream infile{"cities.txt"};
  if (!infile.is_open()) {
    cerr << format("failed to open file cities.txt\n");
    return 1;
  }
  skip_bom(infile);
  for (City c{}; infile >> c;) {
    cities.push_back(c);
  }
  for (const auto &[name, pop, lat, lon] : cities) {
    cout << format("{:.<15} pop: {:<10} coords: {}, {}\n", name, pop, lat, lon);
  }

  // Custom char_traits
  ci_string ci_str1{"Hello, World!"};
  ci_string ci_str2{"hElLo, wOrLd!"};
  cout << "operator<<: " << ci_str1 << "\n";
  cout << format("ci_str1: {}\n", ci_str1);  // need not specialization
  cout << format("ci_str2: {}\n", ci_str2);
  if (ci_str1 == ci_str2) {
    cout << format("ci_str1 == ci_str2\n");
  } else {
    cout << format("ci_str1 != ci_str2\n");
  }
  cout << format("found ci_str2: pos {} char {}\n", ci_str2.find('e'), 'e');
  lc_string lc_str{"Hello, World!"};  // copy method in lc_traits is called
  cout << format("lc_str: {}\n", lc_str);

  // Regular expressions
  const regex link_re{"<a href=\"([^\"]*)\"[^<]*>([^<]*)</a>"};
  string in;
  std::ifstream htmlfile("the-end.html");
  for (string line{}; getline(htmlfile, line);) in += line;
  std::sregex_token_iterator it{in.begin(), in.end(), link_re, {1, 2}};
  get_links(it);
}
