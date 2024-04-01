#include <iostream>
#include <vector>

struct BinaryHeap {
  std::vector<long long> arr;
  std::vector<int> insert;
  std::vector<int> positions;
  int size = 0;

  void Swap(int i, int j) {
    std::swap(positions[insert[i]], positions[insert[j]]);
    std::swap(insert[i], insert[j]);
    std::swap(arr[i], arr[j]);
  }

  void SiftDown(int v) {
    while (2 * v + 1 < size) {
      int min_child = 2 * v + 1;
      if (2 * v + 2 < size && arr[2 * v + 1] > arr[2 * v + 2]) {
        min_child = 2 * v + 2;
      }
      if (arr[v] > arr[min_child]) {
        Swap(v, min_child);
        v = min_child;
      } else {
        break;
      }
    }
  }

  void SiftUp(int v) {
    while (v > 0) {
      if (arr[v] < arr[(v - 1) / 2]) {
        Swap(v, (v - 1) / 2);
        v = (v - 1) / 2;
      } else {
        break;
      }
    }
  }

  void Insert(long long x) {
    arr.push_back(x);
    SiftUp(size++);
  }

  long long GetMin() { return arr[0]; }

  void ExtractMin() {
    Swap(0, --size);
    arr.pop_back();
    positions[insert[size]] = -1;
    insert.pop_back();
    SiftDown(0);
  }

  void DecreaseKey(int ind, long long delta) {
    arr[positions[ind]] -= delta;
    SiftUp(positions[ind]);
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int q, ind;
  long long x;
  std::string cmd;
  std::cin >> q;
  BinaryHeap heap;
  for (int i = 0; i < q; ++i) {
    std::cin >> cmd;
    if (cmd == "insert") {
      std::cin >> x;
      heap.positions.push_back(heap.size);
      heap.insert.push_back(i);
      heap.Insert(x);
    } else {
      heap.positions.push_back(-1);
      if (cmd == "getMin") {
        std::cout << heap.GetMin() << '\n';
      } else if (cmd == "extractMin") {
        heap.ExtractMin();
      } else if (cmd == "decreaseKey") {
        std::cin >> ind >> x;
        heap.DecreaseKey(ind - 1, x);
      }
    }
  }
}