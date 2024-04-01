#include <algorithm>
#include <cstring>
#include <iostream>
#include <stack>
#include <vector>

namespace Constants {
const int kShift = 1;
const int kRealAlphabet = 26;
const int kAlphabet = kRealAlphabet + kShift + 1;
const int kBegin = 96;
const int kStrSep = kBegin + kRealAlphabet + 1;
const int kBeginSeparator = 35;
const char kSeparator = '#';
};  // namespace Constants

class SufArr {
  std::string str_;
  std::vector<int> cl_;
  std::vector<int> pos_;

  std::vector<int> tmp_arr_;
  std::vector<int> tmp_cl_;
  std::vector<int> cnt_;

  std::vector<std::vector<int>> min_;
  std::vector<std::vector<int>> max_;
  std::vector<std::pair<int, int>> fast_pow_;

  int Symb(int ind) const {
    return str_[ind] - (str_[ind] == Constants::kSeparator
                            ? Constants::kBeginSeparator
                            : Constants::kBegin);
  }

  // Вместе с разделительным символом
  int InitLen() const { return str_.size() / 2 + 1; };

  void BaseInit() {
    std::vector<int> cnt_base(Constants::kAlphabet, 0);

    for (int i = 0; i < InitLen(); ++i) {
      ++cnt_base[Symb(i)];
    }
    for (int i = 1; i < Constants::kAlphabet; ++i) {
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

  void MakeLeftRight() {
    MakeBorder(left, false);
    MakeBorder(right, true);
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

  void MakeSparse() {
    //    MakeSparseTable(min_, true, arr);
    //    MakeSparseTable(max_, false, arr);
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

 public:
  std::vector<int> arr;
  std::vector<int> lcp;
  std::vector<int> left;
  std::vector<int> right;
  std::vector<int> str_num;
  std::vector<std::vector<int>> min_lcp;

  SufArr(std::string&& init_str) : str_(std::move(init_str)) {
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
    MakeSparseTable(min_lcp, true, lcp);
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

  char Character(int ind) { return str_[ind]; }

  int MinLCP(int left_ind, int right_ind) {
    std::pair<int, int> pow = fast_pow_[right_ind - left_ind + 1];
    return std::min(min_lcp[pow.first][left_ind],
                    min_lcp[pow.first][right_ind - pow.second + 1]);
  }
};

void FindK(std::string& str, long long num) {
  SufArr suf(str + Constants::kSeparator + str);

  int last = -1;
  long long ans = 0;
  std::pair<int, int> str_ans;
  for (int i = 0; i < suf.Len() - 1; ++i) {
    if (suf.str_num[i] != suf.str_num[i + 1]) {
      if (last == -1) {
        ans += static_cast<long long>(suf.lcp[i]);
      } else {
        ans += std::max(
            0LL, static_cast<long long>(suf.lcp[i] - suf.MinLCP(last, i)));
      }
      if (ans >= num) {
        str_ans = {suf.arr[i], suf.lcp[i] + num - ans};
        break;
      }
      last = i;
    }
  }

  if (ans < num) {
    std::cout << -1;
  }
  for (int i = 0; i < str_ans.second; ++i) {
    std::cout << str[str_ans.first + i];
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::string first;
  std::string second;
  long long num;
  std::cin >> first >> second >> num;

  first += static_cast<char>(Constants::kStrSep) + second;
  FindK(first, num);
}