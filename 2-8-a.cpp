#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using std::vector;

namespace Constants {
const int kMaxDist = 2009000999;
}

struct Graph {
  int vert;
  int edge;
  vector<vector<std::pair<int, int>>> edges;
  vector<int> dist;
  std::set<std::pair<int, int>> set;

  Graph(int vertexes, int edge) : vert(vertexes), edge(edge) {
    edges.resize(vertexes);
    dist.resize(vertexes, Constants::kMaxDist);
  }
};

void Dijkstra(Graph& graph, int vertex) {
  graph.set.erase(graph.set.begin());
  for (auto& to : graph.edges[vertex]) {
    if (graph.dist[to.second] > graph.dist[vertex] + to.first) {
      graph.set.erase({graph.dist[to.second], to.second});
      graph.dist[to.second] = graph.dist[vertex] + to.first;
      graph.set.insert({graph.dist[to.second], to.second});
    }
  }
  if (graph.set.empty()) {
    return;
  }
  Dijkstra(graph, graph.set.begin()->second);
}

void Iteration(int vertexes, int edges) {
  Graph graph(vertexes, edges);
  std::pair<int, int> edge;
  int length;
  int current;
  for (int i = 0; i < edges; ++i) {
    std::cin >> edge.first >> edge.second >> length;
    graph.edges[edge.first].emplace_back(length, edge.second);
    graph.edges[edge.second].emplace_back(length, edge.first);
  }
  std::cin >> current;
  graph.dist[current] = 0;
  graph.set.insert({0, current});
  Dijkstra(graph, current);
  for (auto& dist : graph.dist) {
    std::cout << dist << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  size_t cnt;
  int vertexes;
  int edges;
  std::cin >> cnt;
  for (size_t i = 0; i < cnt; ++i) {
    std::cin >> vertexes >> edges;
    Iteration(vertexes, edges);
  }
}