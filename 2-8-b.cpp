#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>
using std::vector;

namespace Constants {
const long long kMaxDist = 1e18;
}

struct Graph {
  long long vert;
  long long edge;
  vector<vector<std::pair<long long, long long>>> edges;
  vector<long long> dist;
  std::set<std::pair<long long, long long>> set;

  Graph(long long vertexes, long long edge) : vert(vertexes), edge(edge) {
    edges.resize(vertexes);
    dist.resize(vertexes, Constants::kMaxDist);
  }
};

void Dijkstra(Graph& graph, long long vertex) {
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

long long Iteration(long long vertexes, long long edges,
                    vector<long long>& rotten) {
  Graph graph(vertexes, edges);
  std::pair<long long, long long> edge;
  long long length;
  long long begin;
  long long end;
  for (long long i = 0; i < edges; ++i) {
    std::cin >> edge.first >> edge.second >> length;
    graph.edges[edge.first - 1].emplace_back(length, edge.second - 1);
    graph.edges[edge.second - 1].emplace_back(length, edge.first - 1);
  }
  std::cin >> begin >> end;
  --begin;
  --end;
  graph.dist[end] = 0;
  graph.set.insert({0, end});
  Dijkstra(graph, end);
  for (long long evil : rotten) {
    if (graph.dist[evil] <= graph.dist[begin]) {
      return -1;
    }
  }
  return graph.dist[begin];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  long long vertexes;
  long long edges;
  long long evil;
  std::cin >> vertexes >> edges >> evil;
  vector<long long> rotten(evil);
  for (long long i = 0; i < evil; ++i) {
    std::cin >> rotten[i];
    --rotten[i];
  }
  std::cout << Iteration(vertexes, edges, rotten);
}