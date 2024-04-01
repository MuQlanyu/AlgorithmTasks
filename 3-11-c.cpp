#include <algorithm>
#include <cstring>
#include <iostream>
#include <stack>
#include <vector>

namespace Constants {
const int kAlphabet = 11;
const int kBeginSeparator = -1;
const int kBegin = 0;
const int kSeparator = -1;
};  // namespace Constants

class SufArr {
  std::vector<int> str_;
  std::vector<int> cl_;
  std::vector<int> pos_;

  std::vector<int> tmp_arr_;
  std::vector<int> tmp_cl_;
  std::vector<int> cnt_;

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

 public:
  std::vector<int> arr;
  std::vector<int> lcp;
  std::vector<int> left;
  std::vector<int> right;

  SufArr(std::vector<int>&& init_str) : str_(std::move(init_str)) {
    arr.resize(InitLen());
    cl_.resize(InitLen());

    pos_.resize(Len());
    lcp.resize(Len());
    left.resize(Len());
    right.resize(Len());

    tmp_arr_.resize(InitLen());
    tmp_cl_.resize(InitLen());
    cnt_.resize(InitLen());

    BaseInit();
    MakeLcp();
    MakeLeftRight();
  }

  // Без разделительного символа
  int Len() { return InitLen() - 1; }

  int Elem(int ind) { return str_[ind]; }
};

void PrintAns(SufArr& suf) {
  long long tmp;
  long long ans = 0;
  int ans_ind = -1;
  int len = 0;
  long long first;
  long long second;
  for (int i = 0; i < suf.Len(); ++i) {
    first = static_cast<long long>(suf.right[i] - suf.left[i] + 2) *
            static_cast<long long>(suf.lcp[i]);
    second = static_cast<long long>(suf.Len() - suf.arr[i]);
    tmp = std::max(first, second);
    if (ans < tmp) {
      ans = tmp;
      ans_ind = suf.arr[i];
      len = (tmp == first ? suf.lcp[i] : suf.Len() - suf.arr[i]);
    }
  }
  std::cout << ans << '\n';
  std::cout << len << '\n';
  for (int i = 0; i < len; ++i) {
    std::cout << suf.Elem(i + ans_ind) << ' ';
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int num;
  int alpha;
  int tmp;
  std::cin >> num >> alpha;
  std::vector<int> str(2 * num + 1);
  str[num] = -1;
  for (int i = 0; i < num; ++i) {
    std::cin >> tmp;
    str[i] = str[i + num + 1] = tmp;
  }

  SufArr suf(std::move(str));
  PrintAns(suf);
}