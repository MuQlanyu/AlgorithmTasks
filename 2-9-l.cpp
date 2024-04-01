#include <iostream>
#include <vector>
using std::vector;

struct Edge {
  int to;
  int flow;
  int index;
  int back;
  Edge(int edge, int flow, int index, int back) :
      to(edge), flow(flow), index(index), back(back) {}
};

int dfs(int v, int flow, vector<vector<Edge>>& graph, vector<bool>& used) {
  if (v == graph.size() - 1) return flow;
  used[v] = true;
  for (auto& edge : graph[v]) {
    if (used[edge.to] || edge.flow == 0) continue;
    int to_flow = dfs(edge.to, std::min(flow, edge.flow), graph, used);
    if (to_flow > 0) {
      edge.flow -= to_flow;
      graph[edge.to][edge.back].flow += to_flow;
      return to_flow;
    }
  }
  return 0;
}

void dfs_(int v, vector<vector<Edge>>& graph, vector<bool>& used, vector<bool>& ans) {
  ans[v] = used[v] = true;
  for (auto& edge : graph[v]) {
    if (used[edge.to] || edge.flow == 0) continue;
    dfs_(edge.to, graph, used, ans);
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, cost;
  std::pair<int, int> e;
  std::cin >> n >> m;
  vector<vector<Edge>> graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> e.first >> e.second >> cost;
    graph[e.first - 1].emplace_back(e.second - 1, cost, i, graph[e.second - 1].size());
    graph[e.second - 1].emplace_back(e.first - 1, cost, i, graph[e.first - 1].size() - 1);
  }

  int cnt = 0, flow = -1;
  vector<bool> used(n, false);
  while (flow != 0) {
    cnt += flow = dfs(0, 1e9, graph, used);
    std::fill(used.begin(), used.end(), false);
  }
  vector<int> split;
  vector<bool> first(n, false);
  dfs_(0, graph, used, first);
  for (int i = 0; i < n; ++i) {
    if (!first[i]) continue;
    for (auto& elem : graph[i]) {
      if (!first[elem.to]) split.push_back(elem.index);
    }
  }
  std::cout << split.size() << ' ' << cnt << '\n';
  for (int elem : split) {
    std::cout << elem + 1 << ' ';
  }
}