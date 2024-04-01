#include <algorithm>
#include <iostream>
#include <vector>
using std::vector;
vector<int> ans;
bool is_cycle = false;

void Dfs(int v, vector<vector<int>>& g, vector<char>& colour) {
  colour[v] = 'g';
  for (int to : g[v]) {
    if (colour[to] == 'w') {
      Dfs(to, g, colour);
      if (is_cycle) {
        return;
      }
    } else if (colour[to] == 'g') {
      is_cycle = true;
      return;
    }
  }
  ans.push_back(v);
  colour[v] = 'b';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, from, into;
  std::cin >> n >> m;
  vector<vector<int>> g(n, vector<int>(n));
  for (int i = 0; i < m; ++i) {
    std::cin >> from >> into;
    g[from - 1].push_back(into - 1);
  }
  vector<char> colour(n, 'w');
  for (int i = 0; i < n; ++i) {
    if (colour[i] == 'w') {
      Dfs(i, g, colour);
      if (is_cycle) {
        break;
      }
    }
  }
  if (is_cycle) {
    std::cout << -1;
  } else {
    for (size_t i = ans.size(); i > 0; --i) {
      std::cout << ans[i - 1] + 1 << ' ';
    }
  }
}