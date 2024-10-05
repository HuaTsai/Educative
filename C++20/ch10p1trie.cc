#include <bits/stdc++.h>

using namespace std;

class Trie {
 public:
  void insert(const initializer_list<string> &il) {
    insert_(il.begin(), il.end());
  }

  deque<deque<string>> &get() const {
    result_dq_.clear();
    deque<string> dq{};
    get_(dq, result_dq_);
    return result_dq_;
  }

  deque<string> &find_prefix(const char *s) const {
    find_prefix_(s, prefix_dq_);
    return prefix_dq_;
  }

  optional<const Trie *> search(
      const initializer_list<const char *> &il) const {
    return search_(il.begin(), il.end());
  }

  optional<const Trie *> search(const string &s) const {
    const initializer_list<const char *> il{s.c_str()};
    return search_(il.begin(), il.end());
  }

 private:
  // e.g. {"a", "b, "c"}
  // after the recursive call, the trie will look like:
  // {"a": {"b": {"c": {}}}}
  template <typename It>
  void insert_(It it, It end_it) {
    if (it == end_it) return;
    nodes_[*it].insert_(++it, end_it);
  }

  void get_(deque<string> &dq, deque<deque<string>> &result_dq) const {
    if (empty_()) {
      result_dq.push_back(dq);
      dq.clear();
    }
    for (const auto &p : nodes_) {
      dq.push_back(p.first);
      p.second.get_(dq, result_dq);
    }
  }

  void find_prefix_(const string &s, auto &pre_dq) const {
    if (empty_()) return;
    for (const auto &[k, v] : nodes_) {
      if (k.starts_with(s)) {
        pre_dq.emplace_back(k);
        v.find_prefix_(k, pre_dq);
      }
    }
  }

  template <typename It>
  optional<const Trie *> search_(It it, It end_it) const {
    if (it == end_it) return this;
    auto found_it = nodes_.find(*it);
    if (found_it == nodes_.end()) return {};
    return found_it->second.search_(++it, end_it);
  }

  bool empty_() const { return nodes_.empty(); }

  mutable deque<deque<string>> result_dq_{};
  mutable deque<string> prefix_dq_{};
  map<string, Trie> nodes_{};
};

void print_trie_prefix(const Trie &t, const string &prefix) {
  auto &trie_strings = t.get();
  cout << format("results for \"{}...\":\n", prefix);
  for (auto &dq : trie_strings) {
    cout << format("{} ", prefix);
    for (const auto &s : dq) cout << format("{} ", s);
    cout << '\n';
  }
}

void print_trie_prefix(const Trie &t,
                       const initializer_list<const char *> &prefix) {
  string sprefix{};
  for (const auto &s : prefix) sprefix += format("{} ", s);
  print_trie_prefix(t, sprefix);
}

int main() {
  Trie ts;
  ts.insert({"all", "you", "need", "is", "love"});
  ts.insert({"all", "shook", "up"});
  ts.insert({"all", "the", "best"});
  ts.insert({"all", "the", "gold", "in", "california"});
  ts.insert({"at", "last"});
  ts.insert({"love", "the", "one", "you're", "with"});
  ts.insert({"love", "me", "do"});
  ts.insert({"love", "is", "the", "answer"});
  ts.insert({"loving", "you"});
  ts.insert({"long", "tall", "sally"});

  const auto prefix = {"love"};
  if (auto st = ts.search(prefix); st.has_value()) {
    print_trie_prefix(*st.value(), prefix);
  }
  cout << '\n';

  const auto prefix2 = {"all", "the"};
  if (auto st = ts.search(prefix2); st.has_value()) {
    print_trie_prefix(*st.value(), prefix2);
  }
  cout << '\n';

  const char *prefix3{"lo"};
  auto prefix_dq = ts.find_prefix(prefix3);
  for (const auto &s : prefix_dq) {
    cout << format("match: {} -> {}\n", prefix3, s);
    if (auto st = ts.search(s); st.has_value()) {
      print_trie_prefix(*st.value(), s);
    }
  }
  cout << '\n';
}
