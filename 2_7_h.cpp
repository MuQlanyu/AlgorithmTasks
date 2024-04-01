#include <algorithm>
#include <iostream>
#include <vector>
using std::pair;
using std::vector;
vector<int> bridge;
vector<int> points;

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

void Dfs(int v, Graph& graph, vector<int>& answer, vector<int>& bridges) {
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
      bridges.push_back(i.first);
      bridges.push_back(v);
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

void MakeGraphWithoutBridges(Graph& graph, int m,
                             vector<pair<int, int>>& edges) {
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

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  int m;
  std::cin >> n >> m;
  vector<pair<int, int>> edges(m);
  FindBridges(n, m, edges);

  Graph graph(n, m - bridge.size());
  MakeGraphWithoutBridges(graph, m, edges);

  vector<int> component(n);
  int comp_num = SplitComponents(graph, n, component);

  vector<int> comps(comp_num, 0);
  for (auto& i : points) {
    ++comps[component[i]];
  }

  int ans = 0;
  for (auto& i : comps) {
    if (i == 1) {
      ++ans;
    }
  }
  std::cout << (ans + 1) / 2;
}