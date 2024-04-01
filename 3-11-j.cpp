#include <algorithm>
#include <cstring>
#include <iostream>
#include <stack>
#include <vector>

namespace Constants {
const int kRealAlphabet = 26;
const int kAlphabet = kRealAlphabet + 1;
const int kBegin = -1;
const int kStrSep = kBegin + kRealAlphabet + 1;
const int kBeginSeparator = 0;
const int kSeparator = 0;
const int kSymbBegin = 96;
};  // namespace Constants

class SufArr {
  struct Node {
    int val;
    int left = -1;
    int right = -1;
  };

  int alphabet = Constants::kAlphabet;
  int num;

  std::vector<int> str_;
  std::vector<int> cl_;
  std::vector<int> pos_;

  std::vector<int> tmp_arr_;
  std::vector<int> tmp_cl_;
  std::vector<int> cnt_;

  std::vector<std::vector<int>> min_;
  std::vector<std::vector<int>> max_;
  std::vector<std::pair<int, int>> fast_pow_;

  std::vector<std::vector<Node>> fractional;

  int Symb(int ind) const {
    return str_[ind] - (str_[ind] == Constants::kSeparator
                        ? Constants::kBeginSeparator
                        : Constants::kBegin);
  }

  // Вместе с разделительным символом
  int InitLen() const { return str_.size() / 2 + 1; };

  void BaseInit() {
    std::vector<int> cnt_base(alphabet, 0);

    for (int i = 0; i < InitLen(); ++i) {
      ++cnt_base[Symb(i)];
    }
    for (int i = 1; i < alphabet; ++i) {
      cnt_base[i] += cnt_base[i - 1];
    }
    for (int i = InitLen() - 1; i >= 0; --i) {
      arr[--cnt_base[Symb(i)]] = i;
    }

    ClassInit();
  }

  void ClassInit() {
    cl_[arr[0]] = 0;
    for (int i = 1; i < InitLen(); ++i) {
      cl_[arr[i]] = cl_[arr[i - 1]];
      if (str_[arr[i]] != str_[arr[i - 1]]) {
        ++cl_[arr[i]];
      }
    }
    StepInit(2);
  }

  void StepInit(int step) {
    for (int i = 0; i < InitLen(); ++i) {
      tmp_arr_[i] = arr[i] - step / 2;
      if (tmp_arr_[i] < 0) {
        tmp_arr_[i] += InitLen();
      }
    }

    std::fill(cnt_.begin(), cnt_.end(), 0);
    for (int i = 0; i < InitLen(); ++i) {
      ++cnt_[cl_[i]];
    }
    for (int i = 1; i < InitLen(); ++i) {
      cnt_[i] += cnt_[i - 1];
    }
    for (int i = InitLen() - 1; i >= 0; --i) {
      arr[--cnt_[cl_[tmp_arr_[i]]]] = tmp_arr_[i];
    }
    if (step < InitLen()) {
      ClassStepInit(step);
    } else {
      std::vector<int> tmp(Len());
      for (int i = 0; i < Len(); ++i) {
        tmp[i] = arr[i + 1];
      }
      arr = std::move(tmp);
    }
  }

  void ClassStepInit(int step) {
    tmp_cl_[arr[0]] = 0;
    for (int i = 1; i < InitLen(); ++i) {
      tmp_cl_[arr[i]] = tmp_cl_[arr[i - 1]];
      if (cl_[arr[i]] != cl_[arr[i - 1]]) {
        ++tmp_cl_[arr[i]];
      } else if (cl_[Ind(arr[i] + step / 2)] !=
          cl_[Ind(arr[i - 1] + step / 2)]) {
        ++tmp_cl_[arr[i]];
      }
    }
    std::memcpy(cl_.data(), tmp_cl_.data(), InitLen() * sizeof(cl_[0]));

    StepInit(2 * step);
  }

  int Ind(int num) const {
    num %= 2 * InitLen() - 1;
    return num % InitLen();
  }

  void MakeLcp() {
    for (int i = 0; i < Len(); ++i) {
      pos_[arr[i]] = i;
    }

    int cnt = 0;
    int tmp;
    lcp[Len() - 1] = 0;
    for (int i = 0; i < Len(); ++i) {
      cnt -= (cnt > 0 ? 1 : 0);
      if (pos_[i] == Len() - 1) {
        continue;
      }
      tmp = arr[pos_[i] + 1];
      while (str_[i + cnt] == str_[tmp + cnt]) {
        ++cnt;
      }
      lcp[pos_[i]] = cnt;
    }
  }

  void MakeBorder(std::vector<int>& border, bool is_reversed) {
    std::stack<int> stack;
    int begin = (!is_reversed ? 0 : Len() - 1);
    int shift = (!is_reversed ? 1 : -1);
    int ind = begin;
    while (ind >= 0 && ind < Len()) {
      while (!stack.empty() && lcp[stack.top()] >= lcp[ind]) {
        stack.pop();
      }
      border[ind] = (stack.empty() ? begin : stack.top() + shift);
      stack.push(ind);
      ind += shift;
    }
  }

  void MakeSparseTable(std::vector<std::vector<int>>& vec, bool is_min,
                       std::vector<int> source) {
    vec.emplace_back(std::vector<int>(Len()));
    for (int i = 0; i < Len(); ++i) {
      vec[0][i] = source[i];
    }
    int ind = 1;
    int base = 1;
    int end;
    while (2 * base <= Len()) {
      vec.emplace_back(std::vector<int>(Len()));
      for (int i = 0; i < Len(); ++i) {
        end = vec[ind - 1][(i + base < Len() ? i + base : Len() - 1)];
        vec[ind][i] = (vec[ind - 1][i] < end == is_min ? vec[ind - 1][i] : end);
      }
      ++ind;
      base *= 2;
    }
  }

  void MakeLeftRight() {
    MakeBorder(left, false);
    MakeBorder(right, true);
  }

  void MakeSparse() {
    int pow = 1;
    int pow_ind = 0;
    fast_pow_[0] = {0, 1};
    for (int i = 1; i < Len() + 1; ++i) {
      if (i > 2 * pow) {
        pow *= 2;
        ++pow_ind;
      }
      fast_pow_[i] = {pow_ind, pow};
    }
  }

  void MakeStrNum() {
    int ind = 1;
    for (int i = 0; i < Len(); ++i) {
      if (str_[i] >= Constants::kStrSep) {
        for (int j = 0; j < i; ++j) {
          if (str_num[pos_[j]] == -1) {
            str_num[pos_[j]] = ind;
          }
        }
        ++ind;
        str_num[pos_[i]] = 0;
      }
    }
    for (int j = 0; j < Len(); ++j) {
      if (str_num[j] == -1) {
        str_num[j] = ind;
      }
    }
  }

  void MakeNext() {
    next.resize(Len());
    int sz = static_cast<int>(str_num.size());
    std::vector<int> last(alphabet + Constants::kRealAlphabet, sz);
    for (int i = sz - 1; i >= 0; --i) {
      next[i] = last[str_num[i]];
      last[str_num[i]] = i;
    }
  }

  int FindMinCascading(int goal, int ind, int layer, int boarder) {
    while(ind >= boarder && goal >= fractional[layer][ind].val) {
      --ind;
    }
    return (ind < boarder ? -1 : ind);
  }

  void MakeFractionalCascading() {
    int sz = fast_pow_[next.size()].second * 2;
    int ind = fast_pow_[next.size()].first + 1;
    fractional.resize(ind + 1, std::vector<Node>(sz, {sz * 2}));
    for (int i = 0; i < next.size(); ++i) {
      fractional[0][i].val = next[i];
    }

    int base = 1;
    int left = 0;
    int right = 0;
    int boarder = 0;
    int iter = 0;
    for (int i = 1; i <= ind; ++i) {
      base *= 2;
      iter = 0;
      boarder = 0;
      while (boarder <= sz) {
        boarder += base;
        left = boarder - base;
        right = boarder - base / 2;
        while (left < boarder - base / 2 && right < boarder) {
          if (fractional[i - 1][left].val > fractional[i - 1][right].val) {
            fractional[i][iter].val = fractional[i - 1][left].val;
            fractional[i][iter].left = FindMinCascading(fractional[i][iter].val, left++, i - 1, boarder - base);
            fractional[i][iter++].right = FindMinCascading(fractional[i][iter].val, right, i - 1, boarder - base / 2);
          } else if (fractional[i - 1][left].val == fractional[i - 1][right].val) {
            fractional[i][iter].val = fractional[i - 1][right].val;
            fractional[i][iter].left = FindMinCascading(fractional[i][iter].val, left++, i - 1, boarder - base);
            fractional[i][iter++].right = FindMinCascading(fractional[i][iter].val, right, i - 1, boarder - base / 2);
          } else {
            fractional[i][iter].val = fractional[i - 1][right].val;
            fractional[i][iter].left = FindMinCascading(fractional[i][iter].val, left, i, boarder - base);
            fractional[i][iter++].right = FindMinCascading(fractional[i][iter].val, right++, i - 1, boarder - base / 2);
          }
        }
        while (left < boarder - base / 2) {
          fractional[i][iter].val = fractional[i - 1][left].val;
          fractional[i][iter].left = FindMinCascading(fractional[i][iter].val, left++, i - 1, boarder - base);
          fractional[i][iter++].right = FindMinCascading(fractional[i][iter].val, right, i - 1, boarder - base / 2);
        }
        while (right < boarder) {
          fractional[i][iter].val = fractional[i - 1][right].val;
          fractional[i][iter].left = FindMinCascading(fractional[i][iter].val, left, i - 1, boarder - base);
          fractional[i][iter++].right = FindMinCascading(fractional[i][iter].val, right++, i - 1, boarder - base / 2);
        }
      }
    }
  }

  int FindG(int ind, int boarder) {
    int layer = fast_pow_[next.size()].first + 1;
    int left = -1;
    int right = fractional[layer].size() - 1;
    int mid;
    while (right - left > 1) {
      mid = (left + right) / 2;
      if (fractional[layer][mid].val >= boarder) {
        left = mid;
      } else {
        right = mid;
      }
    }

    int base = fast_pow_[next.size()].second;
    int cur = right;
    int ans = 1;  //учитываю момент, когда спускаемся на самый низ
    int sep = base;
    while (layer > 0) {
      if (sep <= ind) {
        ans += 1 + fractional[layer - 1][cur].left % base;
        cur = fractional[layer--][cur].right;
        base /= 2;
        sep += base;
      } else {
        cur = fractional[layer--][cur].left + 1;
        base /= 2;
        sep -= base;
      }
      if (cur == -1) {
        break;
      }
    }
    return ans;
  }

 public:
  std::vector<int> arr;
  std::vector<int> lcp;
  std::vector<int> left;
  std::vector<int> right;
  std::vector<int> str_num;
  std::vector<int> next;
  std::vector<std::vector<int>> min_lcp;

  SufArr(std::vector<int>&& init_str, int num) : str_(std::move(init_str)), num(num) {
    alphabet += num - 1;
    arr.resize(InitLen());
    cl_.resize(InitLen());

    pos_.resize(Len());
    lcp.resize(Len());
    left.resize(Len());
    right.resize(Len());
    fast_pow_.resize(Len() + 1);
    str_num.resize(Len(), -1);

    tmp_arr_.resize(InitLen());
    tmp_cl_.resize(InitLen());
    cnt_.resize(InitLen());

    BaseInit();
    MakeLcp();
    MakeLeftRight();
    MakeSparse();
    MakeStrNum();
    //    MakeSparseTable(min_lcp, true, lcp);
    MakeNext();
    MakeFractionalCascading();
  }

  // Без разделительного символа
  int Len() { return InitLen() - 1; }

  int Get(int left_ind, int right_ind, bool is_min) {
    std::pair<int, int> pow = fast_pow_[right_ind - left_ind + 1];
    if (is_min) {
      return std::min(min_[pow.first][left_ind],
                      min_[pow.first][right_ind - pow.second + 1]);
    }
    return std::max(max_[pow.first][left_ind],
                    max_[pow.first][right_ind - pow.second + 1]);
  }

  int Character(int ind) { return str_[ind]; }

  int MinLCP(int left_ind, int right_ind) {
    std::pair<int, int> pow = fast_pow_[right_ind - left_ind + 1];
    return std::min(min_lcp[pow.first][left_ind],
                    min_lcp[pow.first][right_ind - pow.second + 1]);
  }

  int FindNum(int left_ind, int right_ind) {
    return FindG(right_ind, right_ind) - (left_ind == 0 ? 0 : FindG(left_ind - 1, right_ind));
  }
};

void VectorFill(std::string& str, std::vector<int>& res) {
  int last = static_cast<int>(res.size());
  res.resize(res.size() + str.size());
  for (int i = 0; i < str.size(); ++i) {
    res[last + i] = str[i] - Constants::kSymbBegin;
  }
}

void FindCommon(std::vector<int>& str, int num) {
  SufArr suf(std::move(str), num + 1);
  int left = 0;
  int right = 0;
  int fill;
  std::vector<int> cnt(num + 1, 0);
  for (int i = 0; i < suf.Len(); ++i) {
    if (suf.str_num[i] == 0) {
      break;
    }
    left = suf.left[i];
    right = suf.right[i];
    fill = suf.FindNum(left, std::min(right + 1, suf.Len()));
    cnt[fill] = std::max(cnt[fill], suf.lcp[i]);
  }

  for (int i = 2; i < num + 1; ++i) {
    std::cout << cnt[i] << '\n';
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int num;
  std::string str;
  std::vector<int> res;
  std::cin >> num >> str;
  int separator = Constants::kStrSep;

  VectorFill(str, res);
  for (int i = 1; i < num; ++i) {
    res.push_back(separator++);
    std::cin >> str;
    VectorFill(str, res);
  }

  int sz = static_cast<int>(res.size());
  res.push_back(Constants::kSeparator);
  res.resize(2 * res.size() - 1);
  for (int i = 0; i < sz; ++i) {
    res[sz + 1 + i] = res[i];
  }

  FindCommon(res, num);
}