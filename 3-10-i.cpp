// no_concepts
#include <deque>
#include <iostream>
#include <vector>

namespace MagicNumbers {
const int kAlphabetLen = 2;
const char kFirst = '0';
const char kLast = '1';
}  // namespace MagicNumbers

class Trie {
  struct Node {
    bool term = false;
    int branch[MagicNumbers::kAlphabetLen] = {-1, -1};
    int GetNode(char symbol) { return branch[symbol - MagicNumbers::kFirst]; }
  };

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

 public:
  void AddStr(std::string& str) {
    int cur = 0;
    for (auto symbol : str) {
      if (to_[cur].term) {
        break;
      }
      if (to_[cur].GetNode(symbol) == -1) {
        to_[cur].branch[symbol - MagicNumbers::kFirst] =
            static_cast<int>(to_.size());
        to_.push_back(Node());
      }
      cur = to_[cur].GetNode(symbol);
    }
    to_[cur].term = true;
    to_[cur].branch[0] = -1;
    to_[cur].branch[1] = -1;
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
        if (to_[link_[son]].term) {
          to_[son].term = true;
          to_[son].branch[0] = -1;
          to_[son].branch[1] = -1;
        }
        for (char sym = MagicNumbers::kFirst; sym <= MagicNumbers::kLast;
             ++sym) {
          Go(son, sym) = (to_[son].GetNode(sym) == -1 ? Go(link_[son], sym)
                                                      : to_[son].GetNode(sym));
        }
        deq.push_back(son);
      }
    }
  }

  bool Solve(int ver, std::vector<char>& colour) {
    colour[ver] = 'g';
    for (char ch = MagicNumbers::kFirst; ch <= MagicNumbers::kLast; ++ch) {
      if (to_[Go(ver, ch)].term) {
        continue;
      }
      if (colour[Go(ver, ch)] == 'w' && Solve(Go(ver, ch), colour)) {
        return true;
      }
      if (colour[Go(ver, ch)] == 'g') {
        return true;
      }
    }
    colour[ver] = 'b';
    return false;
  }

  size_t Size() { return to_.size(); }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::string str;
  int num;
  std::cin >> num;
  Trie trie;
  for (int ii = 0; ii < num; ++ii) {
    std::cin >> str;
    trie.AddStr(str);
  }
  trie.AhoCorasick();

  std::vector<char> colour(trie.Size(), 'w');
  if (trie.Solve(0, colour)) {
    std::cout << "TAK";
  } else {
    std::cout << "NIE";
  }
}