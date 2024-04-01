#include <iostream>
#include <vector>
using std::vector;
using Graph = vector<vector<int>>;

int dfs(int v, Graph& graph, vector<bool>& visited, vector<vector<int>>& edges, int flow) {
  if (v == graph.size() - 1) return flow;
  visited[v] = true;
  for (int to : graph[v]) {
    if (visited[to] || edges[v][to] == 0) continue;
    int to_flow = dfs(to, graph, visited, edges, std::min(flow, edges[v][to]));
    if (to_flow > 0) {
      edges[v][to] -= to_flow;
      edges[to][v] += to_flow;
      return to_flow;
    }
  }
  return 0;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, cost, left, right;
  std::cin >> n >> m;
  Graph graph(n);
  vector<vector<int>> edges(n, vector<int>(n, 0));
  for (int i = 0; i < m; ++i) {
    std::cin >> left >> right >> cost;
    if (edges[left - 1][right - 1] == 0 || edges[right - 1][left - 1] == 0) {
      graph[left - 1].push_back(right - 1);
      graph[right - 1].push_back(left - 1);
    }
    edges[left - 1][right - 1] += cost;
  }
  vector<bool> visited(n, false);
  int cnt = 0, flow = 1;
  while (flow != 0) {
    cnt += flow = dfs(0, graph, visited, edges, 1e8);
    visited.assign(n, false);
  }
  std::cout << cnt;
}