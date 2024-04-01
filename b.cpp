#include <iostream>

struct Node {
  int key = 0;
  int prior = 0;
  Node* parent = nullptr;
  Node* left = nullptr;
  Node* right = nullptr;
};

Node* arr;

void Insert(Node& tmp, int prev) {
  arr[prev].right = &tmp;
  while (tmp.parent != nullptr && tmp.prior < arr[prev].prior) {
    arr[prev].right = tmp.left;
    if (tmp.left != nullptr) {
      tmp.left->parent = &arr[prev];
    }
    tmp.parent = arr[prev].parent;
    if (tmp.parent != nullptr) {
      tmp.parent->right = &tmp;
    }
    tmp.left = &arr[prev];
    arr[prev].parent = &tmp;
    if (tmp.parent != nullptr) {
      prev = tmp.parent->key;
    }
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, a = 1, b;
  std::cin >> n;
  arr = new Node[n];
  std::cin >> a >> b;
  arr[0] = {0, b, nullptr};
  for (int i = 1; i < n; ++i) {
    std::cin >> a >> b;
    arr[i] = {i, b, &arr[i - 1]};
    Insert(arr[i], i - 1);
  }
  std::cout << "YES\n";
  for (int i = 0; i < n; ++i) {
    if (arr[i].parent == nullptr) {
      std::cout << "0 ";
    } else {
      std::cout << arr[i].parent->key + 1 << ' ';
    }
    if (arr[i].left == nullptr) {
      std::cout << "0 ";
    } else {
      std::cout << arr[i].left->key + 1 << ' ';
    }
    if (arr[i].right == nullptr) {
      std::cout << "0\n";
    } else {
      std::cout << arr[i].right->key + 1 << '\n';
    }
  }
  delete[] arr;
}