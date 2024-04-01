#include <iostream>

//Прости за код стайл, яндекс контест требует именно такой
struct Node {
  int value;
  int height = 0;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
};

class AVL {
  void UpdateHeight(Node& vertex) {
    vertex.height = 1;
    if (vertex.left_child != nullptr) {
      vertex.height = vertex.left_child->height + 1;
    }
    if (vertex.right_child != nullptr &&
        vertex.right_child->height > vertex.height - 1) {
      vertex.height = vertex.right_child->height + 1;
    }
  }
  void LeftRotate(Node& rotate_root) {
    if (rotate_root.right_child == nullptr) {
      return;
    }
    Node* right_copy = rotate_root.right_child;
    rotate_root.right_child = right_copy->right_child;
    right_copy->right_child = right_copy->left_child;
    right_copy->left_child = rotate_root.left_child;
    rotate_root.left_child = right_copy;
    std::swap(rotate_root.value, rotate_root.left_child->value);
    UpdateHeight(*rotate_root.left_child);
    UpdateHeight(rotate_root);
  }
  void RightRotate(Node& rotate_root) {
    if (rotate_root.left_child == nullptr) {
      return;
    }
    Node* left_copy = rotate_root.left_child;
    rotate_root.left_child = left_copy->left_child;
    left_copy->left_child = left_copy->right_child;
    left_copy->right_child = rotate_root.right_child;
    rotate_root.right_child = left_copy;
    std::swap(rotate_root.value, rotate_root.right_child->value);
    UpdateHeight(*rotate_root.right_child);
    UpdateHeight(rotate_root);
  }
  void Balance(Node& vertex) {
    UpdateHeight(vertex);
    int delta_height = (vertex.left_child == nullptr ? 0 : vertex.left_child->height);
    delta_height -= (vertex.right_child == nullptr ? 0 : vertex.right_child->height);
    if (delta_height == -2) {
      delta_height = (vertex.right_child->left_child == nullptr ?
                      0 : vertex.right_child->left_child->height);
      delta_height -= (vertex.right_child->right_child == nullptr ?
                       0 : vertex.right_child->right_child->height);
      if (delta_height == 1) {
        RightRotate(*vertex.right_child);
      }
      LeftRotate(vertex);
    } else if (delta_height == 2) {
      delta_height = (vertex.left_child->left_child == nullptr ?
                      0 : vertex.left_child->left_child->height);
      delta_height -= (vertex.left_child->right_child == nullptr ?
                       0 : vertex.left_child->right_child->height);
      if (delta_height == -1) {
        LeftRotate(*vertex.left_child);
      }
      RightRotate(vertex);
    }
  }

  void Destructor(Node* current_vertex) {
    if (current_vertex != nullptr && current_vertex->left_child != nullptr) {
      Destructor(current_vertex->left_child);
    }
    if (current_vertex != nullptr && current_vertex->right_child != nullptr) {
      Destructor(current_vertex->right_child);
    }
    delete current_vertex;
  }

 public:
  Node* root = nullptr;

  Node* Insert(int value, Node* current_vertex) {
    if (current_vertex == nullptr) {
      return new Node({value, 1});
    }
    if (current_vertex->value < value) {
      current_vertex->right_child = Insert(value, current_vertex->right_child);
    } else {
      if (current_vertex->value == value) {
        return current_vertex;
      }
      current_vertex->left_child = Insert(value, current_vertex->left_child);
    }
    Balance(*current_vertex);
    return current_vertex;
  }

  int Find(int find) {
    if (root == nullptr) {
      return -1;
    }
    Node current_vertex = *root;
    int ans = -1;
    while (true) {
      if (current_vertex.value < find) {
        if (current_vertex.right_child == nullptr) {
          return ans;
        }
        current_vertex = *current_vertex.right_child;
      } else {
        ans = current_vertex.value;
        if (current_vertex.left_child == nullptr) {
          return ans;
        }
        current_vertex = *current_vertex.left_child;
      }
    }
  }

  ~AVL() { Destructor(root); }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, num, last_find;
  char cmd;
  bool is_last_find = false;
  std::cin >> n;
  AVL avl;
  for (int i = 0; i < n; ++i) {
    std::cin >> cmd >> num;
    if (cmd == '+') {
      if (is_last_find) {
        avl.root = avl.Insert((num + last_find) % 1'000'000'000, avl.root);
        is_last_find = false;
      } else {
        avl.root = avl.Insert(num, avl.root);
      }
    } else {
      is_last_find = true;
      last_find = avl.Find(num);
      std::cout << last_find << '\n';
    }
  }
}
