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

}
