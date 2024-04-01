#include <iostream>

struct Node {
  int val = -1;
  Node* next = nullptr;
};

int HashFunction(long long val, long long array_size) {
  return static_cast<int>((3 * val + 7) % array_size);
}

void Delete(Node* hash_array, int key, int delete_val) {
  if (hash_array[key].val == delete_val) {
    if (hash_array[key].next == nullptr) {
      hash_array[key] = {};
    } else {
      hash_array[key] = *hash_array[key].next;
    }
    return;
  }
  Node** prev_ptr = &hash_array[key].next;
  Node* cur_ptr = hash_array[key].next;
  while (cur_ptr != nullptr) {
    if (cur_ptr->val == delete_val) {
      *prev_ptr = cur_ptr->next;
      return;
    }
    prev_ptr = &cur_ptr->next;
    cur_ptr = cur_ptr->next;
  }
}

bool Find(Node* cur_ptr, int find_val) {
  while (cur_ptr != nullptr) {
    if (cur_ptr->val == find_val) {
      return true;
    }
    cur_ptr = cur_ptr->next;
  }
  return false;
}

void Insert(Node* hash_array, int key, Node insert) {
  if (Find(&hash_array[key], insert.val)) {
    return;
  }
  if (hash_array[key].val == -1) {
    hash_array[key] = insert;
    return;
  }
  insert.next = &hash_array[key];
  hash_array[key] = insert;
}

Node* ReHash(Node* old_hash_array, int& hash_size) {
  hash_size *= 4;
  Node* new_hash_array = new Node[hash_size];
  int key;
  for (int i = 0; i < hash_size / 4; ++i) {
    if (old_hash_array[i].val != -1) {
      key = HashFunction(old_hash_array[i].val, hash_size);
      Insert(new_hash_array, key, {old_hash_array[i].val});
      Delete(old_hash_array, i, old_hash_array[i].val);
    }
  }
  delete[] old_hash_array;
  return new_hash_array;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, num, key, hash_array_size = 100000, current_size = 0;
  char cmd;
  Node* hash_array = new Node[hash_array_size];
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    std::cin >> cmd >> num;
    key = HashFunction(num, hash_array_size);
    if (cmd == '+') {
      Insert(hash_array, key, {num});
      if (current_size++ > hash_array_size / 4) {
        hash_array = ReHash(hash_array, hash_array_size);
      }
    } else if (cmd == '-') {
      Delete(hash_array, key, num);
      --current_size;
    } else {
      std::cout << (Find(&hash_array[key], num) ? "YES" : "NO") << '\n';
    }
  }
  delete[] hash_array;
}