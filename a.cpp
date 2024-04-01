#include <iostream>
#include <vector>
using std::pair;
using std::vector;

void DistCalculate(vector<vector<int>>& dist, vector<int>& arr, int n) {
  dist.resize(n);
  for (int i = 0; i < n; ++i) {
    dist[i].resize(n);
  }
  int ind;
  for (int i = 0; i < n; ++i) {
    ind = i;
    dist[i][i] = 0;
    for (int j = i + 1; j < n; ++j) {
      dist[i][j] = dist[i][j - 1] + (arr[j] - arr[j - 1]) * (j - ind);
      while (arr[ind] - arr[i] < arr[j] - arr[ind]) {
        dist[i][j] += arr[ind] - arr[i] - arr[j] + arr[ind];
        ++ind;
      }
    }
  }
}

void Print(vector<vector<pair<int, int>>>& dp, vector<int>& arr, int n, int m) {
  int ind = n - 1, ans = dp[n - 1][m - 1].first, add = 0;
  for (int i = n - 2; i >= m - 1; --i) {
    add += (arr[i + 1] - arr[i]) * (n - 1 - i);
    if (dp[i][m - 1].first + add < ans) {
      ans = dp[i][m - 1].first + add;
      ind = i;
    }
  }
  std::cout << ans << '\n';

  vector<int> path;
  path.push_back(ind);
  for (int i = m - 1; i > 0; --i) {
    ind = dp[ind][i].second;
    path.push_back(ind);
  }
  for (size_t i = path.size(); i > 0; --i) {
    std::cout << arr[path[i - 1]] << ' ';
  }
}

void Solve(vector<vector<int>>& dist, vector<int>& arr, int n, int m) {
  vector<vector<pair<int, int>>> dp;
  dp.resize(n);
  int sum = 0;
  for (int i = 1; i < n; ++i) {
    sum += arr[i] - arr[0];
  }
  dp[0].assign(m, {1e8, -1});
  dp[0][0] = {0, -1};
  for (int i = 1; i < n; ++i) {
    dp[i].assign(m, {1e8, -1});
    dp[i][0].first = dp[i - 1][0].first + i * (arr[i] - arr[i - 1]);
  }

  for (int j = 1; j < m; ++j) {
    for (int k = j; k < n; ++k) {
      for (int i = j - 1; i < k; ++i) {
        if (dp[k][j].first > dp[i][j - 1].first + dist[i][k]) {
          dp[k][j] = {dp[i][j - 1].first + dist[i][k], i};
        }
      }
    }
  }

  Print(dp, arr, n, m);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m;
  vector<int> arr;
  std::cin >> n >> m;
  arr.resize(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }

  vector<vector<int>> dist;
  DistCalculate(dist, arr, n);

  Solve(dist, arr, n, m);
}
