#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using std::vector;

namespace Constants {
const int kMaxInt = 1e8;
const int kMaxInput = 1e5;
}  // namespace Constants

struct Graph {
  int vert;
  vector<vector<std::pair<int, int>>> edges;
  vector<vector<int>> dp;
  vector<int> prev;

  Graph(int vertexes) : vert(vertexes) {
    edges.resize(vertexes);
    dp.resize(vert, vector<int>(2 * vert + 1, Constants::kMaxInt));
    prev.resize(vert, 0);
  }
};

void FindNegativeCycle(Graph& graph) {
  vector<int> ans;
  for (int i = 0; i < graph.vert; ++i) {
    if (graph.dp[i][2 * graph.vert] < graph.dp[i][graph.vert - 1]) {
      int vert = i;
      for (int j = 0; j < graph.vert; ++j) {
        vert = graph.prev[vert];
      }
      int begin = vert;
      while (begin != graph.prev[vert]) {
        ans.push_back(vert);
        vert = graph.prev[vert];
      }
      ans.push_back(vert);
      ans.push_back(graph.prev[vert]);
      break;
    }
  }

  if (ans.empty()) {
    std::cout << "NO";
  } else {
    std::cout << "YES\n" << ans.size() << '\n';
    for (size_t i = ans.size(); i > 0; --i) {
      std::cout << ans[i - 1] + 1 << ' ';
    }
  }
}

void Iteration(Graph& graph) {
  graph.dp[0][0] = 0;
  std::pair<int, int> cur_min;
  for (int k = 0; k < 2 * graph.vert; ++k) {
    for (int ver = 0; ver < graph.vert; ++ver) {
      cur_min.first = Constants::kMaxInt;
      for (auto& prev : graph.edges[ver]) {
        if (cur_min.first > graph.dp[prev.first][k] + prev.second) {
          cur_min = {graph.dp[prev.first][k] + prev.second, prev.first};
        }
      }

      if (graph.dp[ver][k] >= cur_min.first) {
        graph.dp[ver][k + 1] = cur_min.first;
        graph.prev[ver] = cur_min.second;
      } else {
        graph.dp[ver][k + 1] = graph.dp[ver][k];
      }
    }
  }
  FindNegativeCycle(graph);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int vertexes;
  int length;
  std::cin >> vertexes;
  Graph graph(vertexes);

  for (int i = 0; i < vertexes; ++i) {
    for (int j = 0; j < vertexes; ++j) {
      std::cin >> length;
      if (length != Constants::kMaxInput) {
        graph.edges[j].emplace_back(i, length);
      }
    }
  }

  if (vertexes == 1) {
    if (!graph.edges[0].empty() && graph.edges[0][0].second < 0) {
      std::cout << "YES\n2\n1 1";
    } else {
      std::cout << "NO";
    }
  } else {
    Iteration(graph);
  }
}