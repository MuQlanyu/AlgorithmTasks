#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>

class SNM {
  std::vector<int> parent, rank;
 public:
  SNM(int n) {
    parent.resize(n);
    rank.resize(n, 0);
    for (int i = 0; i < n; ++i) {
      parent[i] = i;
    }
  }

  int get(int vertex) {
    int root = vertex;
    int tmp;
    while (parent[root] != root) {
      root = parent[root];
    }
    while (vertex != parent[vertex]) {
      tmp = parent[vertex];
      parent[vertex] = root;
      vertex = tmp;
    }
    return root;
  }

  void merge(int left, int right) {
    int rleft = get(left);
    int rright = get(right);
    if (rleft == rright) return;
    if (rank[rleft] == rank[rright]) {
      ++rank[rleft];
      parent[rright] = rleft;
    } else if (rank[rleft] < rank[rright]) {
      parent[rleft] = rright;
    } else {
      parent[rright] = rleft;
    }
  }
};

void Solve(int n, std::vector<std::pair<int, std::pair<int, int>>>& edges) {
  SNM snm(n);
  int total_cost = 0;
  for (auto edge : edges) {
    if (snm.get(edge.second.first) != snm.get(edge.second.second)) {
      total_cost += edge.first;
      snm.merge(edge.second.first, edge.second.second);
    }
  }
  std::cout << total_cost;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, cost;
  std::pair<int, int> edge;
  std::cin >> n >> m;
  std::vector<std::pair<int, std::pair<int, int>>> edges(m);
  for (int i = 0; i < m; ++i) {
    std::cin >> edge.first >> edge.second >> cost;
    edges[i] = {cost, {edge.first - 1, edge.second - 1}};
  }
  std::sort(edges.begin(), edges.end());
  Solve(n, edges);
}