#include <bits/stdc++.h>

using namespace std;

void printc(auto& r) {
  cout << format("size({}) ", r.size());
  for (auto& e : r) cout << format("{} ", e);
  cout << endl;
}

void print_assoc(auto& r) {
  cout << format("size: {}: ", r.size());
  for (auto& [k, v] : r) cout << format("{}:{} ", k, v);
  cout << "\n";
}

struct Point {
  int x, y;
  Point(int x, int y) : x(x), y(y) {}
  bool operator==(const Point& p) const { return x == p.x && y == p.y; }
};

template <>
struct std::hash<Point> {
  size_t operator()(const Point& p) const {
    return hash<int>()(p.x) ^ hash<int>()(p.y);
  }
};

class RPN {
  deque<double> deq_{};
  constexpr static double zero_{0.0};
  constexpr static double inf_{std::numeric_limits<double>::infinity()};

 public:
  double op(const string& s) {
    if (is_numeric(s)) {
      double v{stod(s, nullptr)};
      deq_.push_front(v);
      return v;
    } else {
      return optor(s);
    }
  }

  void clear() { deq_.clear(); }

  string get_stack_string() const {
    string s{};
    for (auto v : deq_) {
      s += format("{} ", v);
    }
    return s;
  }

 private:
  pair<double, double> pop_get2() {
    if (deq_.size() < 2) return {zero_, zero_};
    double v1{deq_.front()};
    deq_.pop_front();
    double v2{deq_.front()};
    deq_.pop_front();
    return {v2, v1};
  }

  bool is_numeric(const string& s) {
    for (const char c : s) {
      if (c != '.' && !std::isdigit(c)) return false;
    }
    return true;
  }

  double optor(const string& op) {
    map<string, double (*)(double, double)> opmap{
        {"+", [](double l, double r) { return l + r; }},
        {"-", [](double l, double r) { return l - r; }},
        {"*", [](double l, double r) { return l * r; }},
        {"/", [](double l, double r) { return l / r; }},
        {"^", [](double l, double r) { return pow(l, r); }},
        {"%", [](double l, double r) { return fmod(l, r); }}};

    if (opmap.find(op) == opmap.end()) return zero_;
    auto [l, r] = pop_get2();

    if (op == "/" && r == zero_)
      deq_.push_front(inf_);
    else
      deq_.push_front(opmap.at(op)(l, r));
    return deq_.front();
  }
};

int main() {
  // Erase-remove idiom -> directly remove in C++20
  vector<int> c = {1, 2, 3, 4, 5};

  // Before C++20:
  // auto remove_it = std::remove(c.begin(), c.end(), 3);
  // c.erase(remove_it, c.end());
  // printc(c);

  // After C++20:
  erase(c, 3);
  printc(c);

  // Predicate version
  erase_if(c, [](int x) { return x % 2 == 0; });
  printc(c);

  map<int, string> m = {{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};
  print_assoc(m);

  erase_if(m, [](auto& p) {
    auto& [k, v] = p;
    return k % 2 == 0;
  });
  print_assoc(m);

  // O(1) delete
  // 1. index or iterator is known
  // 2. no need to maintain order
  c = {12, 196, 47, 38, 19};
  auto quick_delete_idx = [](auto &c, int idx) {
    if (idx < c.size()) {
      c[idx] = move(c.back());
      c.pop_back();
    }
  };
  auto quick_delete_it = [](auto &c, auto it) {
    if (it < c.end()) {
      *it = move(c.back());
      c.pop_back();
    }
  };
  quick_delete_idx(c, 2);
  auto it = ranges::find(c, 196);
  quick_delete_it(c, it);
  printc(c);

  // Insert sorted
  vector<int> v = {9, 1, 5, 3, 7};
  ranges::sort(v);
  printc(v);
  auto insert_sorted = [](auto &v, int x) {
    auto it = ranges::lower_bound(v, x);
    v.insert(it, x);
  };
  insert_sorted(v, 4);
  printc(v);

  // Insert elements to a map by emplace
  struct BigThing {
    string v_;
    BigThing() { cout << "default ctor\n"; }  // needed for operator[]
    BigThing(string v) : v_(v) { cout << format("ctor: {}\n", v); }
  };
  map<string, BigThing> mp;

  // 1. Use emplace instead of []
  // Important: we should always use emplace instead of [] in modern C++
  mp["one"] = BigThing("one");  // two ctors: lhs does not exist so calls default ctor, rhs calls ctor
  mp.emplace("two", "two");     // one ctor: faster

  // 2. If the key exists, emplace will fail
  // To avoid creating unnecessary object, use try_emplace (C++17)
  // Therefore, we should always favor try_emplace over emplace
  mp.emplace("two", "second two");  // Fail, but still calls ctor
  if (auto [i, s] = mp.try_emplace("two", "third two"); !s) {   // Fail, no payload ctor called
    cout << "Failed to emplace\n";
  }

  // Modify key of map: extract in C++17
  map<int, string> m1 = {{1, "Mario"}, {2, "Luigi"}, {3, "Bowser"}};
  auto node1 = m1.extract(1);  // the key is writable for node
  auto node2 = m1.extract(2);
  print_assoc(m1);
  swap(node1.key(), node2.key());
  m1.insert(move(node1));
  m1.insert(move(node2));
  print_assoc(m1);

  // extract returns a node_handle object, we can check the emptiness
  if (auto node = m1.extract(4); node.empty()) {
    cout << "node 4 is empty\n";
  }

  // iterator can be used to extract
  if (auto node = m1.extract(m1.find(2)); !node.empty()) {
    cout << "node 2 is found\n";
  }

  // inserted can be used to check if the key is inserted successfully
  // std::pair<iterator, bool> insert(const value_type& value);
  if (auto [it, inserted] = m1.insert({4, "Peach"}); inserted) {
    cout << "4 is inserted\n";
  }

  // insert_return_type insert(node_type&& nh);
  if (auto status = m1.insert(move(m1.extract(m1.begin()))); status.inserted) {
    cout << "m1.begin() is extracted and then inserted\n";
  }

  // custom key of unordered_map
  // unordered_map<Point, int>  -> use hash specialization, more general
  // unordered_map<Point, int, hash_func_type> -> use hash function
  unordered_map<Point, int> ump { {{0, 0}, 1}, {{1, 1}, 2}, {{2, 2}, 3} };
  for (const auto &[k, v] : ump) {
    cout << format("{{ ({}, {}), {} }} ", k.x, k.y, v);
  }
  cout << endl;

  // Inserter for set
  set<string> wd;
  cout << "Enter words: ";
  istream_iterator<string> isit(cin);
  istream_iterator<string> end;  // default eof
  copy(isit, end, inserter(wd, wd.end()));
  cout << "Words: " << wd.size() << endl;
  for (auto &w : wd) cout << w << " ";
  cout << endl;

  // RPN Calculator
  RPN rpn;
  vector<string> opv{"9", "6", "*", "2", "3", "*", "+"};
  for(auto o : opv) {
    rpn.op(o);
    auto stack_str{rpn.get_stack_string()};
    cout << format("{}: {}\n", o, stack_str);
  }

  // Word frequency
  map<string, int> wordmap{};
  vector<pair<string, int>> wordvec{};
  regex word_re(R"((\w+))");  // R"delim ... delim", (): capture group

  ifstream ifs("input.txt");
  size_t total_words{};
  for (string s{}; ifs >> s; ) {  // may be better than while loop, which s is declared outside
    sregex_iterator words_begin(s.begin(), s.end(), word_re);
    sregex_iterator words_end;
    for(auto r_it{words_begin}; r_it != words_end; ++r_it) {
      smatch match{*r_it};
      auto word_str{match.str()};
      ranges::transform(word_str, word_str.begin(), [](unsigned char c) { return tolower(c); });
      // Important:
      //   old method -> check if the key exists, if not insert 1, else increment
      //   new method -> try_emplace, get reference and increment
      auto [map_it, result] = wordmap.try_emplace(word_str, 0);
      auto &[w, count] = *map_it;
      ++total_words;
      ++count;
    }
  }

  auto unique_words = wordmap.size();
  wordvec.reserve(unique_words);
  ranges::move(wordmap, back_inserter(wordvec));
  ranges::sort(wordvec, [](const auto& a, const auto& b) { 
    return (a.second != b.second) ? (a.second > b.second) : (a.first < b.first);
  });

  cout << format("total word count: {}\n", total_words);
  cout << format("unique word count: {}\n", unique_words);

  for(int limit{20}; auto& [w, count] : wordvec) {
    cout << format("{}: {}\n", count, w);
    if(--limit == 0) break;
  }
}
