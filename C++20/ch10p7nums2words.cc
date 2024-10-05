#include <bits/stdc++.h>

using namespace std;

// maxnum is nine hundred ninety-nine quadrillion nine hundred ninety-nine
// trillion nine hundred ninety-nine billion nine hundred ninety-nine million
// nine hundred ninety-nine thousand nine hundred ninety-nine
constexpr uint64_t maxnum = 999'999'999'999'999'999;
constexpr int zero_i{0};
constexpr int five_i{5};
constexpr uint64_t zero{0};
constexpr uint64_t ten{10};
constexpr uint64_t twenty{20};
constexpr uint64_t hundred{100};
constexpr uint64_t thousand{1000};
constexpr string_view errnum{"error"};
constexpr string_view _singles[]{"zero", "one", "two",   "three", "four",
                                 "five", "six", "seven", "eight", "nine"};
constexpr string_view _teens[]{"ten",      "eleven",  "twelve",  "thirteen",
                               "fourteen", "fifteen", "sixteen", "seventeen",
                               "eighteen", "nineteen"};
constexpr string_view _tens[]{
    errnum,  errnum,  "twenty",  "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety",
};
constexpr string_view _hundred_string = "hundred";
constexpr string_view _powers[]{errnum,    "thousand", "million",
                                "billion", "trillion", "quadrillion"};
constexpr char _hyphen{'-'};
constexpr char _space{' '};

class NumWord {
 public:
  NumWord(uint64_t num = zero) : _num{num} {}
  NumWord(const NumWord &nw) : _num(nw.getnum()) {}

  void setnum(uint64_t num) { _num = num; }
  uint64_t getnum() const { return _num; }
  const string &operator()(const uint64_t &num) { return words(num); };
  uint64_t operator=(const uint64_t &num) {
    setnum(num);
    return getnum();
  }

  const string &words() { return words(_num); }

  const string &words(const uint64_t &num) {
    uint64_t n{num};
    clearbuf();

    if (n > maxnum) {
      appendbuf(errnum);
      return *_buf;
    }

    if (n == 0) {
      appendbuf(_singles[n]);
      return *_buf;
    }

    // powers of 1000
    if (n >= thousand) {
      for (int i{five_i}; i > zero_i; --i) {
        uint64_t power{pow_i(thousand, i)};
        uint64_t _n{(n - (n % power)) / power};
        if (_n) {
          int index = i;
          NumWord _nw{_n};
          appendbuf(_nw.words());
          appendbuf(_powers[index]);
          n -= _n * power;
        }
      }
    }

    // hundreds
    if (n >= hundred && n < thousand) {
      uint64_t _n{(n - (n % hundred)) / hundred};
      NumWord _nw{_n};
      appendbuf(_nw.words());
      appendbuf(_hundred_string);
      n -= _n * hundred;
    }

    // tens
    if (n >= twenty && n < hundred) {
      uint64_t _n{(n - (n % ten)) / ten};
      appendbuf(_tens[_n]);
      n -= _n * ten;
      _hyphen_flag = true;
    }

    // teens
    if (n >= ten && n < twenty) {
      appendbuf(_teens[n - ten]);
      n = zero;
    }

    // singles
    if (n > zero && n < ten) {
      appendbuf(_singles[n]);
    }

    return *_buf;
  }

 private:
  void clearbuf() { *_buf = string{}; }

  size_t bufsize() { return _buf->size(); }

  void appendbuf(const string &s) {
    appendspace();
    _buf->append(s);
  }

  void appendbuf(const string_view &s) {
    appendspace();
    _buf->append(s.data());
  }

  void appendbuf(const char c) { _buf->append(1, c); }

  void appendspace() {
    if (bufsize()) {
      appendbuf(_hyphen_flag ? _hyphen : _space);
      _hyphen_flag = false;
    }
  }

  uint64_t pow_i(const uint64_t n, uint64_t p) {
    uint64_t out{n};
    while (--p) out *= n;
    return out;
  }

  std::unique_ptr<string> _buf{std::make_unique<string>(string{})};
  uint64_t _num{};
  bool _hyphen_flag{false};
};

template <>
struct std::formatter<NumWord> : formatter<unsigned> {
  template <typename FormatContext>
  auto format(const NumWord &nw, FormatContext &ctx) const {
    NumWord n{nw};
    return format_to(ctx.out(), "{}", n.words());
  }
};

template <typename... Args>
constexpr void print(const std::string_view str_fmt, Args &&...args) {
  fputs(vformat(str_fmt, make_format_args(args...)).c_str(), stdout);
}

int main() {
  NumWord nw{};
  uint64_t n{};

  print("n is {}, {}\n", nw.getnum(), nw);

  nw = 3;
  print("n is {}, {}\n", nw.getnum(), nw);
  nw = 47;
  print("n is {}, {}\n", nw.getnum(), nw);
  nw = 73;
  print("n is {}, {}\n", nw.getnum(), nw);
  nw = 1492;
  print("n is {}, {}\n", nw.getnum(), nw);
  nw = 10012;
  print("n is {}, {}\n", nw.getnum(), nw);

  n = 100073;
  print("n is {}, {}\n", n, NumWord{n});
  n = 1000000;
  print("n is {}, {}\n", n, NumWord{n});
  n = 1000000001;
  print("n is {}, {}\n", n, NumWord{n});
  n = 123000000000;
  print("n is {}, {}\n", n, NumWord{n});

  n = 474142398123;
  print("n is {}, {}\n", n, nw(n));
  n = 1474142398007;
  print("n is {}, {}\n", n, nw(n));
  n = 999000000000000;
  print("n is {}, {}\n", n, nw(n));
  n = 999999999999999999;
  print("n is {}, {}\n", n, nw(n));
  n = 1000000000000000000;
  print("n is {}, {}\n", n, nw(n));
}
