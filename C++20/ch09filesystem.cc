#include <bits/stdc++.h>

using namespace std;
namespace fs = std::filesystem;

template <>
struct std::formatter<fs::path> : formatter<string> {
  template <typename FormatContext>
  auto format(const fs::path &p, FormatContext &ctx) const {
    return format_to(ctx.out(), "{}", p.string());
  }
};

char type_char(const fs::file_status &fstat) {
  if (is_symlink(fstat))
    return 'l';
  else if (is_directory(fstat))
    return 'd';
  else if (is_character_file(fstat))
    return 'c';
  else if (is_block_file(fstat))
    return 'b';
  else if (is_fifo(fstat))
    return 'p';
  else if (is_socket(fstat))
    return 's';
  else if (is_other(fstat))
    return 'o';
  else if (is_regular_file(fstat))
    return '-';
  return '?';
}

string rwx(const fs::perms &p) {
  using fs::perms;
  auto bit2char = [&p](perms bit, char c) {
    return (p & bit) == perms::none ? '-' : c;
  };
  return {bit2char(perms::owner_read, 'r'),  bit2char(perms::owner_write, 'w'),
          bit2char(perms::owner_exec, 'x'),  bit2char(perms::group_read, 'r'),
          bit2char(perms::group_write, 'w'), bit2char(perms::group_exec, 'x'),
          bit2char(perms::others_read, 'r'), bit2char(perms::others_write, 'w'),
          bit2char(perms::others_exec, 'x')};
}

// fsize in bytes
string size_string(const uintmax_t fsize) {
  constexpr const uintmax_t kilo{1024};
  constexpr const uintmax_t mega{kilo * kilo};
  constexpr const uintmax_t giga{mega * kilo};

  // round
  if (fsize >= giga)
    return format("{}{}", (fsize + giga / 2) / giga, 'G');
  else if (fsize >= mega)
    return format("{}{}", (fsize + mega / 2) / mega, 'M');
  else if (fsize >= kilo)
    return format("{}{}", (fsize + kilo / 2) / kilo, 'K');
  else
    return format("{}B", fsize);
}

// directory_entry information
void print_de(const fs::directory_entry &dir) {
  const auto fpath{dir.path()};
  // symlink_status => lstat(2): symlinks are not followed
  const auto fstat{dir.symlink_status()};
  const auto fperm{fstat.permissions()};
  // uintmax_t: maximum size natural integer on the system
  // My Ubuntu 24.04: both size_t and uintmax_t are unsigned long (64-bits)
  const uintmax_t fsize{is_regular_file(fstat) ? file_size(fpath) : 0};
  const auto fn{fpath.filename()};
  string suffix{};
  if (is_symlink(fstat)) {
    suffix = " -> ";
    suffix += fs::read_symlink(fpath).string();
  } else if (is_directory(fstat)) {
    suffix = "/";
  } else if ((fperm & fs::perms::owner_exec) != fs::perms::none) {
    suffix = "*";
  }
  const auto permstr{type_char(fstat) + rwx(fperm)};
  auto sizestr = size_string(fsize);
  // clang-format off
  auto timepoint = chrono::clock_cast<chrono::system_clock>(dir.last_write_time());
  // floor timepoint by seconds duration to a new timepoint
  auto timepoint2 = floor<chrono::seconds>(timepoint);
  cout << format("{} {:>6} {:%F %T} {} {}\n", permstr, sizestr, timepoint2, fn, suffix);
  // clang-format on
}

// simple grep
vector<pair<size_t, string>> matches(const fs::path &path, const regex &re) {
  vector<pair<size_t, string>> ret;
  ifstream ifs{path};
  string s;
  for (size_t i{1}; getline(ifs, s); ++i)
    if (regex_search(s.begin(), s.end(), re)) ret.emplace_back(i, s);
  return ret;
}

// epath: result of a directory search
// spath: the search directory itself
size_t pmatches(const regex &re, const fs::path &epath, const fs::path &spath) {
  auto regmatches = matches(epath, re);
  if (!regmatches.size()) return 0;
  // target = path of how to reach epath from spath
  // if epath is the same as spath, target is "."
  fs::path target = epath.lexically_relative(spath);
  for (const auto &[line, text] : regmatches)
    cout << format("{} {}: {}\n", target, line, text);
  return regmatches.size();
}

string replace_str(string s, const vector<pair<regex, string>> &repl) {
  for (const auto &[re, rep] : repl) s = regex_replace(s, re, rep);
  return s;
}

// disk usage counter: recursive function
uintmax_t entry_size(const fs::path &p) {
  if (fs::is_regular_file(p)) return fs::file_size(p);
  uintmax_t accum{};
  if (fs::is_directory(p) && !fs::is_symlink(p))
    for (auto &e : fs::directory_iterator{p}) accum += entry_size(e.path());
  return accum;
}

int main() {
  // use custom formatter for path
  fs::path p("./ch09filesystem.cc");
  cout << format("path: {}\n", p);
  cout << format("stem: {}\n", p.stem());
  cout << format("filename: {}\n", p.filename());
  cout << format("extension: {}\n", p.extension());
  cout << format("parent_path: {}\n", p.parent_path());
  cout << format("current_path: {}\n", fs::current_path());
  cout << format("fs::absolute(p): {}\n", fs::absolute(p));
  cout << format("fs::cannonical(p): {}\n", fs::canonical(p));
  cout << "loop through fs::canonical: ";
  for (auto &x : fs::canonical(p)) cout << format("[{}] ", x);
  cout << endl;

  // operations
  cout << format("fs::path(\"foo\") += \"bar\": {}\n",
                 fs::path{"foo"} += "bar");
  cout << format("fs::path(\"foo\") /= \"bar\": {}\n",
                 fs::path{"foo"} /= "bar");
  cout << format("equivalent: {}\n",
                 fs::equivalent(fs::absolute(p), fs::canonical(p)));
  try {
    auto res = fs::equivalent(fs::path{"foo"}, fs::path{"bar"});
  } catch (const fs::filesystem_error &e) {
    cout << format("exception: {}\n", e.what());
    cout << format("path1: {}\n", e.path1());
    cout << format("path2: {}\n", e.path2());
  }
  error_code e;
  cout << format("canonical: {}\n", fs::canonical(p /= "foo", e));
  cout << format("error: {}\n", e.message());

  // list files in a directory
  vector<fs::directory_entry> v;
  for (const auto &de : fs::directory_iterator(fs::current_path())) {
    // de is directory_entry, de.exists() and de.is_directory() are available
    v.emplace_back(de);
  }
  ranges::sort(v);
  for (const auto &de : v) {
    print_de(de);
  }

  // recursively search directories and files with grep utility
  regex re;
  try {
    // case-insensitive search
    re = regex("path", regex_constants::icase);
  } catch (const regex_error &e) {
    cout << format("regex_error: {}\n", e.what());
  }
  int matches{0};
  for (const auto &de : fs::recursive_directory_iterator(fs::current_path())) {
    matches += pmatches(re, de.path(), fs::current_path());
  }

  // rename files with regex
  vector<pair<regex, string>> repl{{regex(R"(\.cc$)"), ".cpp"}};
  for (const auto &de : fs::directory_iterator(fs::current_path())) {
    fs::path fpath{de.path()};
    string rname{replace_str(fpath.filename(), repl)};
    if (fpath.filename().string() != rname) {
      fs::path rpath{fpath};
      rpath.replace_filename(rname);
      if (exists(rpath)) {
        cout << "Error: cannot rename - destination file exists.\n";
      } else {
        // comment out real rename
        // fs::rename(fpath, rpath);
        cout << format("{} -> {}\n", fpath.filename(), rpath.filename());
      }
    }
  }

  // Disk usage counter
  fs::path inc{"/usr/include"};
  vector<fs::directory_entry> v2;
  for (const auto &de : fs::directory_iterator(inc))
    v2.emplace_back(de);
  ranges::sort(v2);
  uintmax_t accum{};
  for (const auto &de : v2) {
    uintmax_t esize{entry_size(de.path())};
    string dir_flag{};
    accum += esize;
    if (is_directory(de.path()) && !is_symlink(de.path())) dir_flag = " ▽";
    cout << format("{:>5} {}{}\n", size_string(esize), de.path(), dir_flag);
  }
  cout << format("{:->25}\n", "");
  cout << format("total bytes: {} ({})\n", accum, size_string(accum));
}
