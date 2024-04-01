// no_concepts
#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <vector>

using Vec = std::vector<std::vector<int>>;

namespace MagicNumbers {
const int kAlphabetLen = 26;
const char kFirst = 'a';
const char kLast = 'z';
}  // namespace MagicNumbers

Vec Transposition(Vec& arr) {
  Vec ans(arr[0].size(), std::vector<int>(arr.size()));
  for (size_t ii = 0; ii < arr[0].size(); ++ii) {
    for (size_t jj = 0; jj < arr.size(); ++jj) {
      ans[ii][jj] = arr[jj][ii];
    }
  }
  return ans;
}

class Trie {
  struct Node {
    int term = -1;
    int branch[MagicNumbers::kAlphabetLen];
    int GetNode(char symbol) { return branch[symbol - MagicNumbers::kFirst]; }
    Node() { std::memset(branch, -1, MagicNumbers::kAlphabetLen * 4); }
  };

  std::pair<int, int> size_;
  std::vector<int> array_;
  int last_in_array_ = 0;

  std::vector<Node> to_ = std::vector<Node>(1);
  std::vector<int> link_;
  std::vector<std::vector<int>> suf_;

  int& Go(int vertex, char symbol) {
    return suf_[vertex][symbol - MagicNumbers::kFirst];
  }

  void AhoCorasickBase() {
    link_.resize(to_.size());
    suf_.resize(to_.size(), std::vector<int>(MagicNumbers::kAlphabetLen, 0));

    link_[0] = 0;
    for (char sym = MagicNumbers::kFirst; sym <= MagicNumbers::kLast; ++sym) {
      if (to_[0].GetNode(sym) != -1) {
        Go(0, sym) = to_[0].GetNode(sym);
      } else {
        Go(0, sym) = 0;
      }
    }
  }

  void ZFunction(std::vector<int>& func, const std::vector<int>& str) {
    int left = 0;
    int right = 0;
    int sz = static_cast<int>(str.size());
    for (int ii = 1; ii < sz; ++ii) {
      if (left <= ii && ii <= right) {
        func[ii] = std::max(0, std::min(func[ii - left], right - ii));
      }
      while (ii + func[ii] < sz && str[func[ii]] == str[func[ii] + ii]) {
        ++func[ii];
      }
      if (ii + func[ii] > right) {
        right = func[ii] + ii;
        left = ii;
      }
    }
  }

  void ProcessZ(Vec& arr) {
    std::vector<int> z_func[2];
    z_func[0].resize(arr.size() + 1 + array_.size());
    z_func[1].resize(arr.size() + 1 + array_.size());
    z_func[0][0] = z_func[1][0] = 0;

    std::vector<int> str(arr.size() + 1 + array_.size());
    for (size_t col = 0; col < arr[0].size(); ++col) {
      std::fill(z_func[0].begin(), z_func[0].end(), 0);
      std::fill(z_func[1].begin(), z_func[1].end(), 0);

      std::copy(array_.begin(), array_.end(), str.data());
      str[array_.size()] = -16;
      for (size_t row = 0; row < arr.size(); ++row) {
        str[array_.size() + 1 + row] = arr[row][col];
      }
      ZFunction(z_func[0], str);

      for (size_t row = 0; row < arr.size(); ++row) {
        str[row] = arr[row][col];
      }
      str[arr.size()] = -16;
      std::copy(array_.begin(), array_.end(), str.data() + arr.size() + 1);
      std::reverse(str.begin(), str.end());  // оптимизация
      ZFunction(z_func[1], str);

      for (int row = 0; row + size_.first - 1 < static_cast<int>(arr.size());
           ++row) {
        if (z_func[0][array_.size() + 1 + row] +
                z_func[1][static_cast<int>(array_.size() + 1 + arr.size()) -
                          row - size_.first] >=
            static_cast<int>(array_.size()) - 1) {
          arr[row][col] = -2;
        }
      }
    }
  }

 public:
  Trie(int width, int height) : size_(width, height) {}

  void AddStr(const std::string& str) {
    int cur = 0;
    for (auto symbol : str) {
      if (to_[cur].GetNode(symbol) == -1) {
        to_[cur].branch[symbol - MagicNumbers::kFirst] =
            static_cast<int>(to_.size());
        to_.push_back(Node());
      }
      cur = to_[cur].GetNode(symbol);
    }

    if (to_[cur].term == -1) {
      to_[cur].term = last_in_array_++;
    }
    array_.push_back(to_[cur].term);
  }

  void AhoCorasick() {
    AhoCorasickBase();

    std::deque<int> deq;
    deq.push_back(0);
    int ver;
    int son;
    while (!deq.empty()) {
      ver = deq.front();
      deq.pop_front();
      for (char ch = MagicNumbers::kFirst; ch <= MagicNumbers::kLast; ++ch) {
        son = to_[ver].GetNode(ch);
        if (son == -1) {
          continue;
        }
        link_[son] = (ver == 0 ? 0 : Go(link_[ver], ch));
        for (char sym = MagicNumbers::kFirst; sym <= MagicNumbers::kLast;
             ++sym) {
          Go(son, sym) = (to_[son].GetNode(sym) == -1 ? Go(link_[son], sym)
                                                      : to_[son].GetNode(sym));
        }
        deq.push_back(son);
      }
    }
  }

  void ProcessEntry(Vec& city) {
    int ver = 0;
    std::pair<int, int> size = {static_cast<int>(city.size()),
                                static_cast<int>(city[0].size())};
    for (int row = 0; row < size.first; ++row) {
      ver = 0;
      for (int col = 0; col < size.second; ++col) {
        ver = Go(ver, city[row][col]);
        city[row][col] = (to_[ver].term == -1 ? -1 : to_[ver].term);
      }
    }
    ProcessZ(city);
  }
};

std::string StrReverse(std::vector<std::string>& arr, int ind) {
  std::string ans;
  ans.resize(arr.size());
  for (size_t ii = 0; ii < arr.size(); ++ii) {
    ans[ii] = arr[ii][ind];
  }
  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int n_moscow;
  int m_moscow;
  std::cin >> n_moscow >> m_moscow;
  Vec moscow(n_moscow, std::vector<int>(m_moscow));
  std::string str;
  for (int ii = 0; ii < n_moscow; ++ii) {
    std::cin >> str;
    for (int jj = 0; jj < m_moscow; ++jj) {
      moscow[ii][jj] = static_cast<int>(str[jj]);
    }
  }
  Vec t_moscow = Transposition(moscow);

  int n_mipt;
  int m_mipt;
  std::cin >> n_mipt >> m_mipt;
  Trie trie(n_mipt, m_mipt);
  Trie t_trie(m_mipt, n_mipt);
  std::vector<std::string> mipt(n_mipt);
  for (int ii = 0; ii < n_mipt; ++ii) {
    std::cin >> mipt[ii];
    trie.AddStr(mipt[ii]);
  }

  for (int ii = 0; ii < m_mipt; ++ii) {
    t_trie.AddStr(StrReverse(mipt, ii));
  }

  trie.AhoCorasick();
  t_trie.AhoCorasick();

  trie.ProcessEntry(moscow);
  t_trie.ProcessEntry(t_moscow);

  int ans = 0;
  for (int row = 0; row + n_mipt <= n_moscow; ++row) {
    for (int col = 0; col + m_mipt <= m_moscow; ++col) {
      if (moscow[row][col + m_mipt - 1] == -2 &&
          t_moscow[col][row + n_mipt - 1] == -2) {
        ans += 1;
      }
    }
  }

  std::cout << ans;
}