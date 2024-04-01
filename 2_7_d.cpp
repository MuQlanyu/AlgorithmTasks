#include <iostream>
#include <vector>
using std::vector;

struct Graph {
  int n;
  int m;
  vector<vector<int>> graph;
  vector<char> colour;
};

void Dfs(int v, Graph& graph, vector<int>& ans) {
  graph.colour[v] = 'g';
  for (int i : graph.graph[v]) {
    if (graph.colour[i] == 'w') {
      Dfs(i, graph, ans);
    }
  }
  ans.push_back(v);
  graph.colour[v] = 'b';
}

void DfsR(int v, Graph& graph, vector<int>& components, int ind) {
  graph.colour[v] = 'g';
  components[v] = ind;
  for (int i : graph.graph[v]) {
    if (graph.colour[i] == 'w') {
      DfsR(i, graph, components, ind);
    }
  }
  graph.colour[v] = 'b';
}

void AnsPrint(int n, int num, vector<int>& components) {
  std::cout << num << '\n';
  for (int i = 0; i < n; ++i) {
    std::cout << components[i] + 1 << ' ';
  }
}

void Kasaraju(int n, Graph& graph, Graph& reversed_graph) {
  vector<int> top_sort;
  for (int i = 0; i < n; ++i) {
    if (graph.colour[i] == 'w') {
      Dfs(i, graph, top_sort);
    }
  }

  int component_num = 0;
  vector<int> components(n);
  for (int i = n - 1; i >= 0; --i) {
    if (reversed_graph.colour[top_sort[i]] == 'w') {
      DfsR(top_sort[i], reversed_graph, components, component_num++);
    }
  }
  AnsPrint(n, component_num, components);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n;
  int m;
  int from;
  int to;
  std::cin >> n >> m;
  Graph graph = {n, m, vector<vector<int>>(n), vector<char>(n, 'w')};
  Graph reversed_graph = {n, m, vector<vector<int>>(n), vector<char>(n, 'w')};
  for (int i = 0; i < m; ++i) {
    std::cin >> from >> to;
    graph.graph[from - 1].push_back(to - 1);
    reversed_graph.graph[to - 1].push_back(from - 1);
  }
  Kasaraju(n, graph, reversed_graph);
}