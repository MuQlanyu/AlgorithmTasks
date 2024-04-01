#include <iostream>
#include <vector>
#include <set>
using std::vector;
using Edge = std::pair<long long, long long>;

long long Flow(vector<vector<Edge>>& graph, vector<long long>& parent) {
  long long cur = graph.size() - 1;
  long long total = 0;
  while (cur != 0) {
    if (parent[cur] < 0) return -1;
    if (graph[parent[cur]][cur].first <= 0) continue;
    total += graph[parent[cur]][cur].second;
    --graph[parent[cur]][cur].first;
    ++graph[cur][parent[cur]].first;
    cur = parent[cur];
  }
  return total;
}

std::pair<long long, long long> FordBellman(vector<vector<Edge>>& graph,
                                            vector<vector<Edge>>& edge) {
  vector<vector<long long>> dp(edge.size(), vector<long long>(edge.size() + 1, 1e18));
  dp[0][0] = 0;
  vector<long long> parent(edge.size(), -1);
  parent[0] = 0;
  long long cur_parent = -1;
  long long cur_min;
  for (long long k = 1; k < edge.size() + 1; ++k) {
    for (long long ver = 0; ver < edge.size(); ++ver) {
      cur_min = 1e18;
      for (auto& prev : edge[ver]) {
        if (graph[prev.first][ver].second <= 0) continue;
        if (cur_min > dp[prev.first][k - 1] + prev.second) cur_parent = prev.first;
        cur_min = std::min(cur_min, dp[prev.first][k - 1] + prev.second);
      }
      if (dp[ver][k - 1] > cur_min) parent[ver] = cur_parent;
      dp[ver][k] = std::min(dp[ver][k - 1], cur_min);
    }
  }

  for (long long i = 0; i < graph.size(); ++i) {
    for (long long j = 0; j < graph.size(); ++j) {
      graph[i][j].second += dp[i][graph.size()] - dp[j][graph.size()];
    }
  }
  return {Flow(graph, parent), dp[graph.size() - 1][graph.size()]};
}

void Dijkstra(long long v, vector<vector<Edge>>& graph, vector<long long>& dist,
              std::set<std::pair<long long, long long>>& set, vector<long long>& parent) {
  set.erase(set.begin());
  for (long long i = 0; i < graph.size(); ++i) {
    if (graph[v][i].first <= 0 || graph[v][i].second < 0) continue;
    if (dist[i] > dist[v] + graph[v][i].second) {
      parent[i] = v;
      set.erase({dist[i], i});
      dist[i] = dist[v] + graph[v][i].second;
      set.insert({dist[i], i});
    }
  }
  if (set.empty()) {
    return;
  }
  Dijkstra(set.begin()->second, graph, dist, set, parent);
}
std::pair<long long, long long> Potentials(vector<vector<Edge>>& graph, vector<long long>& dist, vector<long long>& parent) {
  std::fill(dist.begin(), dist.end(), 1e18);
  std::fill(parent.begin(), parent.end(), -1);
  parent[0] = 0;
  dist[0] = 0;
  std::set<std::pair<long long, long long>> set;
  set.insert({0, 0});

  Dijkstra(0, graph, dist, set, parent);
  for (long long i = 0; i < graph.size(); ++i) {
    for (long long j = 0; j < graph.size(); ++j) {
      graph[i][j].second += dist[i] - dist[j];
    }
  }
  return {Flow(graph, parent), dist[graph.size() - 1]};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  long long n, m, left, right, cost, flow;
  std::cin >> n >> m;
  vector<vector<Edge>> graph(n, vector<Edge>(n, {0, 0}));
  vector<vector<Edge>> edge(n);
  for (long long i = 0; i < m; ++i) {
    std::cin >> left >> right >> flow >> cost;
    if (graph[left - 1][right - 1].first == 0 && graph[right - 1][left - 1] == 0) {
      graph[left - 1][right - 1] = {flow, cost};
      graph[right - 1][left - 1] = {0, -cost};
    } else {
      graph[left - 1][right - 1].first += flow;
    }
    edge[right - 1].emplace_back(left - 1, cost);
  }
  std::pair<long long, long long> p = FordBellman(graph, edge);
  if (p.first < 0) {
    std::cout << 0;
    return 0;
  }
  long long total = p.second + p.first, current = -1, delta = p.second;
  vector<long long> dist(n);
  vector<long long> parent(n);
  while (current != 0) {
    p = Potentials(graph, dist, parent);
    if (p.first < 0) break;
    delta += p.second;
    total += current = delta + p.first;
  }
  std::cout << total;
}