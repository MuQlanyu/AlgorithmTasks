#include <iostream>
#include <vector>
using std::pair;
using std::vector;

bool Bit(int mask, int i) { return ((mask >> i) & 1) != 0; }

int RemoveBit(int mask, int i) { return mask ^ (1 << i); }

void Print(vector<vector<pair<int, int>>>& dp, int n) {
  int ind = 0, mask = (1 << n) - 1, tmp;
  for (int i = 1; i < n; ++i) {
    if (dp[mask][i].first < dp[mask][ind].first) {
      ind = i;
    }
  }
  std::cout << dp[mask][ind].first << '\n';
  vector<int> ans;
  for (int i = 0; i < n; ++i) {
    ans.push_back(ind);
    tmp = ind;
    ind = dp[mask][ind].second;
    mask = RemoveBit(mask, tmp);
  }
  for (size_t i = ans.size(); i > 0; --i) {
    std::cout << ans[i - 1] + 1 << ' ';
  }
}

void Solve(vector<vector<int>>& arr, vector<vector<pair<int, int>>>& dp,
           int n) {
  for (int i = 0; i < (1 << n); ++i) {
    dp[i].assign(n, {1e8, -1});
  }
  for (int i = 0; i < n; ++i) {
    dp[0][i] = {0, -1};
  }
  int max_size = 1;
  for (int i = 0; i < n; ++i) {
    dp[max_size][i] = {0, -1};
    max_size <<= 1;
  }
  for (int mask = 1; mask < max_size; ++mask) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (i != j && Bit(mask, j)) {
          if (dp[mask][i].first > dp[RemoveBit(mask, i)][j].first + arr[i][j]) {
            dp[mask][i].first = dp[RemoveBit(mask, i)][j].first + arr[i][j];
            dp[mask][i].second = j;
          }
        }
      }
    }
  }
  Print(dp, n);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  std::cin >> n;
  vector<vector<int>> arr;
  arr.resize(n);
  for (int i = 0; i < n; ++i) {
    arr[i].resize(n);
    for (int j = 0; j < n; ++j) {
      std::cin >> arr[i][j];
    }
  }
  vector<vector<pair<int, int>>> dp;
  dp.resize(1 << n);
  Solve(arr, dp, n);
}