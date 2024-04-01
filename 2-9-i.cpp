#include <iostream>
#include <vector>
using std::vector;
using Graph = vector<vector<int>>;

void dfs(int v, Graph& graph, vector<bool>& visited) {
  visited[v] = true;
  for (auto to : graph[v]) {
    if (visited[to]) continue;
    dfs(to, graph, visited);
  }
}

void print(vector<int>& v, int delta) {
  std::cout << v.size() << ' ';
  for (auto elem : v) {
    std::cout << elem - delta<< ' ';
  }
  std::cout << '\n';
}

void GetEdges(vector<vector<int>>& edges, int n) {
  int num, to;
  for (int i = 0; i < n; ++i) {
    std::cin >> num;
    for (int j = 0; j < num; ++j) {
      std::cin >> to;
      edges[i].push_back(n + to - 1);
    }
  }
}

void MakeGraph(Graph& graph, vector<vector<int>>& edges, vector<int>& mpair) {
  for (int i = 0; i < edges.size(); ++i) {
    if (mpair[i] != 0) graph[edges.size() + mpair[i] - 1].push_back(i);
    for (auto elem : edges[i]) {
      if (elem == edges.size() + mpair[i] - 1) continue;
      graph[i].push_back(elem);
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m;
  std::cin >> n >> m;
  vector<vector<int>> edges(n);
  GetEdges(edges, n);

  vector<int> mpair(n);
  vector<bool> parsoch(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> mpair[i];
    if (mpair[i] != 0) {
      parsoch[i] = true;
    }
  }

  Graph graph(n + m);
  MakeGraph(graph, edges, mpair);

  vector<bool> visited(n + m, false);
  for (int i = 0; i < n; ++i) {
    if (parsoch[i]) continue;
    dfs(i, graph, visited);
  }
  vector<int> left, right;
  for (int i = 0; i < n + m; ++i) {
    if (i < n && !visited[i]) {
      left.push_back(i + 1);
    } else if (i >= n && visited[i]) {
      right.push_back(i + 1);
    }
  }
  std::cout << left.size() + right.size() << '\n';
  print(left, 0);
  print(right, n);
}