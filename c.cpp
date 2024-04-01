#include <algorithm>
#include <iostream>

struct SegTree {
  struct Node {
    int max = -1;
    int max_ind = 0;
    int left = 0;
    int right = 0;
    int med = 0;
  };

  Node* arr = nullptr;
  int n = 0;

  SegTree(int* leafs, int leafs_num, int num) : arr(new Node[num * 2]), n(num) {
    for (int i = 0; i < leafs_num; ++i) {
      arr[i + num] = {leafs[i], i, i, i};
    }
    for (int i = leafs_num; i < num; ++i) {
      arr[i + num] = {0, i, i, i};
    }
    for (int i = num - 1; i > 0; --i) {
      if (arr[2 * i].max > arr[2 * i + 1].max) {
        arr[i].max = arr[2 * i].max;
        arr[i].max_ind = arr[2 * i].max_ind;
      } else {
        arr[i].max = arr[2 * i + 1].max;
        arr[i].max_ind = arr[2 * i + 1].max_ind;
      }
      arr[i].left = arr[2 * i].left, arr[i].right = arr[2 * i + 1].right;
      arr[i].med = (arr[i].left + arr[i].right) / 2;
    }
  }

  void Set(int ind, int val) {
    ind += n;
    arr[ind].max = val;
    ind /= 2;
    while (ind > 0) {
      if (arr[ind * 2].max > arr[ind * 2 + 1].max) {
        arr[ind].max = arr[ind * 2].max;
        arr[ind].max_ind = arr[ind * 2].max_ind;
      } else {
        arr[ind].max = arr[ind * 2 + 1].max;
        arr[ind].max_ind = arr[ind * 2 + 1].max_ind;
      }
      ind /= 2;
    }
  }

  int Get(int current, int find, int val) {
    if (arr[current].left == arr[current].right) {
      return arr[current].left + 1;
    }
    int ans = -1;
    if (find <= arr[current].med && arr[current * 2].max >= val) {
      ans = Get(current * 2, find, val);
    }
    if (arr[current * 2 + 1].max >= val && ans == -1) {
      ans = Get(2 * current + 1, find, val);
    }
    return ans;
  }

  ~SegTree() { delete[] arr; }
};
int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, m, cmd, ind = 0, x;
  std::cin >> n >> m;
  int* places = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> places[i];
  }
  while ((n - 1) >> ind > 0) {
    ++ind;
  }
  SegTree tree = SegTree(places, n, 1 << ind);
  for (int i = 0; i < m; ++i) {
    std::cin >> cmd >> ind >> x;
    if (cmd == 0) {
      tree.Set(ind - 1, x);
    } else {
      std::cout << tree.Get(1, ind - 1, x) << '\n';
    }
  }
  delete[] places;
}