#include <cstring>
#include <iostream>
#include <vector>

namespace Constants {
const int kAlphabet = 27;
const int kHashTag = 35;
const int kBegin = 96;
};  // namespace Constants

class SufArr {
  std::string str_;
  std::vector<int> cl_;

  std::vector<int> tmp_arr_;
  std::vector<int> tmp_cl_;
  std::vector<int> cnt_;

  int Symb(int ind) const {
    return str_[ind] -
           (str_[ind] == '#' ? Constants::kHashTag : Constants::kBegin);
  }

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

 public:
  std::vector<int> arr;

  SufArr(const std::string& init_str) : str_(init_str) {
    arr.resize(InitLen());
    cl_.resize(InitLen());

    tmp_arr_.resize(InitLen());
    tmp_cl_.resize(InitLen());
    cnt_.resize(InitLen());
    BaseInit();
  }

  int Len() { return InitLen() - 1; }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::string str;
  std::cin >> str;
  str = str + "#" + str;

  SufArr arr(str);
  for (int elem : arr.arr) {
    std::cout << elem + 1 << ' ';
  }
}