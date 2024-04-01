#include <cmath>
#include <iostream>
using std::pair;

class SparseTable {
  struct Node {
    pair<int, int> first;
    pair<int, int> second;
  };
  Node** arr_;
  int length_ = 0;
  int max_pow_ = 0;

  bool Check(pair<int, int> left, pair<int, int> right) {
    return (left != right && left.first < right.first);
  }

  void SecondMin(Node& insert, pair<int, int> left, pair<int, int> right) {
    if (Check(left, right) && insert.first != left) {
      insert.second = left;
    } else {
      insert.second = right;
    }
  }

  Node& MakeNode(Node& insert, Node& left, Node& right) {
    if (Check(left.first, right.first)) {
      insert.first = left.first;
      SecondMin(insert, left.second, right.first);
    } else {
      insert.first = right.first;
      if (left.first == right.first) {
        SecondMin(insert, right.second, left.second);
      } else {
        SecondMin(insert, right.second, left.first);
      }
    }
    return insert;
  }

 public:
  SparseTable(int len, int max_pow)
      : arr_(new Node*[max_pow + 1]), length_(len), max_pow_(max_pow) {
    int new_element, pow = 1;
    arr_[0] = new Node[length_];
    for (int i = 0; i < length_; ++i) {
      std::cin >> new_element;
      arr_[0][i] = {{new_element, i}, {new_element, i}};
    }
    for (int row = length_ - 1; row > 0; row -= std::pow(2, pow++)) {
      arr_[pow] = new Node[row];
      for (int i = 0; i < row; ++i) {
        MakeNode(arr_[pow][i], arr_[pow - 1][i],
                 arr_[pow - 1][i + static_cast<int>(std::pow(2, pow - 1))]);
      }
    }
  }

  int FindStatistics(int left, int right, int pow) {
    Node time_var;
    return MakeNode(time_var, arr_[pow][left],
                    arr_[pow][right - static_cast<int>(std::pow(2, pow)) + 1])
        .second.first;
  }

  ~SparseTable() {
    for (int i = 0; i < max_pow_ + 1; ++i) {
      delete[] arr_[i];
    }
    delete[] arr_;
  }
};

void Power(int* arr, int len) {
  int pow = 0, prev_pow = 1;
  for (int i = 0; i < len; ++i) {
    if (i == 2 * prev_pow) {
      ++pow;
      prev_pow *= 2;
    }
    arr[i] = pow;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, l, r;
  std::cin >> n >> m;
  int* powers = new int[n];
  Power(powers, n);
  SparseTable sparse_table = SparseTable(n, powers[n - 1]);
  for (int i = 0; i < m; ++i) {
    std::cin >> l >> r;
    std::cout << sparse_table.FindStatistics(l - 1, r - 1, powers[r - l])
              << '\n';
  }
  delete[] powers;
}