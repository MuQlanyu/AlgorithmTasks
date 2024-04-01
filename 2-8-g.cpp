#include <algorithm>
#include <iostream>
#include <vector>
using std::vector;

void Floyd(vector<vector<int>>& arr) {
  for (size_t k = 0; k < arr.size(); ++k) {
    for (size_t i = 0; i < arr.size(); ++i) {
      for (size_t j = 0; j < arr.size(); ++j) {
        arr[i][j] = std::max(arr[i][j], arr[i][k] & arr[k][j]);
      }
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int num;
  std::cin >> num;
  vector<vector<int>> arr(num, vector<int>(num));
  for (int i = 0; i < num; ++i) {
    for (int j = 0; j < num; ++j) {
      std::cin >> arr[i][j];
    }
  }
  Floyd(arr);
  for (int i = 0; i < num; ++i) {
    for (int j = 0; j < num; ++j) {
      std::cout << arr[i][j] << ' ';
    }
    std::cout << '\n';
  }
}
