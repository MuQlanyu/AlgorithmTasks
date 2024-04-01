#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

namespace Constants {
const int kAlphabet = 26;
const int kBegin = 97;
};  // namespace Constants

class SufAutomaton {
 public:
  struct Node {
    bool terminal = false;
    int len = 0;
    int first = 0;
    int link = 0;
    std::vector<int> to = std::vector<int>(Constants::kAlphabet, -1);

    int& To(char symb) { return to[symb - Constants::kBegin]; }
  };

 private:
  int Add(char symb, int last) {
    arr.emplace_back(Node());
    int cur = static_cast<int>(arr.size()) - 1;
    arr.back().len = arr[last].len + 1;
    arr.back().first = 0;
    int tmp = last;
    while (tmp != -1 && arr[tmp].To(symb) == -1) {
      arr[tmp].To(symb) = cur;
      tmp = arr[tmp].link;
    }
    if (tmp == -1) {
      return cur;
    }

    AddFoundExtension(cur, tmp, symb);
    return cur;
  }

  void AddFoundExtension(int cur, int tmp, char symb) {
    int to = arr[tmp].To(symb);
    if (arr[to].len == arr[tmp].len + 1) {
      arr.back().link = to;
    } else {
      arr.emplace_back(Node());
      int clone = cur + 1;
      arr.back().len = arr[tmp].len + 1;
      arr.back().first = arr[to].first + arr[to].len - arr.back().len;
      while (tmp != -1 && arr[tmp].To(symb) == to) {
        arr[tmp].To(symb) = clone;
        tmp = arr[tmp].link;
      }
      arr.back().to = arr[to].to;
      arr.back().link = arr[to].link;
      arr[cur].link = arr[to].link = clone;
    }
  }

 public:
  std::vector<Node> arr;

  SufAutomaton(std::string& str) {
    arr.emplace_back(Node());  // корень
    arr.back().link = -1;

    int last = 0;
    for (char ch : str) {
      last = Add(ch, last);
    }

    while (last != -1) {
      arr[last].terminal = true;
      last = arr[last].link;
    }
  }
};

std::pair<int, int> FindLongestPath(SufAutomaton& suf, int ind,
                                    std::string& str) {
  int shift = 0;
  int vertex = 0;
  int to;
  int first_input;
  int ans_input;
  int sz = static_cast<int>(str.size());
  while (shift + ind < sz) {
    to = suf.arr[vertex].To(str[ind + shift]);
    first_input = suf.arr[to].first + suf.arr[to].len - shift - 1;
    if (to != -1 && first_input < ind) {
      vertex = to;
      ++shift;
      ans_input = first_input;
    } else {
      break;
    }
  }
  return {shift, ans_input};
}

void Encrypt(std::string& str) {
  SufAutomaton suf(str);
  std::cout << -1 << ' ' << static_cast<int>(str[0]) << '\n';
  int ind = 1;
  int sz = static_cast<int>(str.size());
  std::pair<int, int> ans;
  while (ind < sz) {
    ans = FindLongestPath(suf, ind, str);
    if (ans.first <= 0) {
      std::cout << -1 << ' ' << static_cast<int>(str[ind++]) << '\n';
    } else {
      std::cout << ans.first << ' ' << ans.second << '\n';
      ind += ans.first;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int num;
  std::string str;
  std::cin >> num;
  for (int i = 1; i <= num; ++i) {
    std::cout << "Case #" << i << ":\n";
    std::cin >> str;
    Encrypt(str);
  }
}