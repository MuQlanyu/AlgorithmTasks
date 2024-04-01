#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using std::pair;
using std::vector;
vector<int> bridge;
vector<std::pair<int, int>> points;

vector<int> depth;
vector<vector<int>> dp;
vector<int> par;

struct Graph {
  int n;
  int m;
  int timer = 0;
  vector<vector<std::pair<int, int>>> graph;
  vector<bool> used;
  vector<int> parent;
  vector<int> tin;
  vector<int> ret;

  Graph(int n, int m) : n(n), m(m) {
    graph.resize(n);
    used = vector<bool>(n, false);
    parent = vector<int>(n, -1);
    tin.resize(n);
    ret.resize(n);
  }
};

void Dfs(int v, Graph& graph, vector<int>& answer, vector<std::pair<int, int>>& bridges) {
  graph.used[v] = true;
  graph.tin[v] = graph.timer++;
  graph.ret[v] = graph.tin[v];
  for (std::pair<int, int>& i : graph.graph[v]) {
    if (i.first == graph.parent[v]) {
      continue;
    }
    if (graph.used[i.first]) {
      graph.ret[v] = std::min(graph.ret[v], graph.tin[i.first]);
      continue;
    }
    graph.parent[i.first] = v;
    Dfs(i.first, graph, answer, bridges);
    graph.ret[v] = std::min(graph.ret[v], graph.ret[i.first]);
    if (graph.ret[i.first] == graph.tin[i.first]) {
      answer.push_back(i.second);
      bridges.emplace_back(i.first, v);
    }
  }
}

void DfsComp(int v, Graph& graph, vector<int>& comp, int num) {
  graph.used[v] = true;
  comp[v] = num;
  for (auto& i : graph.graph[v]) {
    if (!graph.used[i.first]) {
      DfsComp(i.first, graph, comp, num);
    }
  }
}

void FindBridges(int n, int m, vector<pair<int, int>>& edges) {
  std::pair<int, int> e;
  Graph graph(n, m);
  for (int i = 0; i < m; ++i) {
    std::cin >> e.first >> e.second;
    edges[i] = e;
    graph.graph[e.first - 1].push_back({e.second - 1, i});
    graph.graph[e.second - 1].push_back({e.first - 1, i});
  }
  Dfs(0, graph, bridge, points);
  std::sort(bridge.begin(), bridge.end());
}

void MakeGraphWithoutBridges(Graph& graph, int m,vector<pair<int, int>>& edges) {
  size_t index = 0;
  for (int i = 0; i < m; ++i) {
    if (index < bridge.size() && bridge[index] == i) {
      ++index;
      continue;
    }
    graph.graph[edges[i].first - 1].push_back({edges[i].second - 1, 0});
    graph.graph[edges[i].second - 1].push_back({edges[i].first - 1, 0});
  }
}

int SplitComponents(Graph& graph, int n, vector<int>& component) {
  int num = 0;
  for (int i = 0; i < n; ++i) {
    if (!graph.used[i]) {
      DfsComp(i, graph, component, num++);
    }
  }
  return num;
}

/// separate ///

void dfs(int v, vector<vector<int>>& graph, vector<bool>& visited) {
  visited[v] = true;
  for (int to : graph[v]) {
    if (visited[to]) continue;
    depth[to] = depth[v] + 1;
    par[to] = v;
    dfs(to, graph, visited);
  }
}

int lca(int left, int right, int log) {
  if (depth[left] > depth[right]) {
    std::swap(left, right);
  }
  for (int i = log - 1; i >= 0; --i) {
    if (depth[dp[right][i]] >= depth[left]) {
      right = dp[right][i];
    }
  }

  if (left == right) return left;
  for (int i = log - 1; i >= 0; --i) {
    if (dp[left][i] != dp[right][i]) {
      left = dp[left][i];
      right = dp[right][i];
    }
  }
  return par[left];
}

void Solve(int n, vector<vector<int>>& graph, int start, vector<int>& component) {
  vector<bool> visited(n, false);
  depth.resize(n, 0);
  par.resize(n, start);
  dfs(start, graph, visited);

  int log = static_cast<int>(std::log2(n)) + 3;
  dp.resize(n, vector<int>(log));
  for (int i = 0; i < n; ++i) {
    dp[i][0] = par[i];
  }
  for (int j = 1; j < log; ++j) {
    for (int i = 0; i < n; ++i) {
      dp[i][j] = dp[dp[i][j - 1]][j - 1];
    }
  }

  int q, left, right;
  std::cin >> q;
  for (int i = 0; i < q; ++i) {
    std::cin >> left >> right;
    std::cout << depth[lca(component[left - 1], component[right - 1], log)] << '\n';
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, f;
  std::cin >> n >> m >> f;
  vector<pair<int, int>> edges(m);
  FindBridges(n, m, edges);

  Graph graph(n, m - bridge.size());
  MakeGraphWithoutBridges(graph, m, edges);

  vector<int> component(n);
  int comp_num = SplitComponents(graph, n, component);

  vector<vector<int>> final_graph(comp_num);
  for (auto& edge : points) {
    final_graph[component[edge.first]].push_back(component[edge.second]);
    final_graph[component[edge.second]].push_back(component[edge.first]);
  }
  Solve(comp_num, final_graph, component[f - 1], component);
}