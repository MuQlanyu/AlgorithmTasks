#include <iostream>
int n;
int* sums;

int F(int x) { return x & (x + 1); }

int G(int x) { return x | (x + 1); }

void Update(int x, int y, int z, int delta) {
  for (int i = x; i < n; i = G(i)) {
    for (int j = y; j < n; j = G(j)) {
      for (int e = z; e < n; e = G(e)) {
        sums[i * n * n + j * n + e] += delta;
      }
    }
  }
}

long long Get(int x, int y, int z) {
  long long ans = 0;
  for (int i = x; i >= 0; i = F(i) - 1) {
    for (int j = y; j >= 0; j = F(j) - 1) {
      for (int e = z; e >= 0; e = F(e) - 1) {
        ans += sums[i * n * n + j * n + e];
      }
    }
  }
  return ans;
}

long long Count(int x1, int y1, int z1, int x2, int y2, int z2) {
  long long ans = Get(x2, y2, z2);
  ans -= x1 > 0 ? Get(x1 - 1, y2, z2) : 0;
  ans -= y1 > 0 ? Get(x2, y1 - 1, z2) : 0;
  ans -= z1 > 0 ? Get(x2, y2, z1 - 1) : 0;
  ans += x1 > 0 && y1 > 0 ? Get(x1 - 1, y1 - 1, z2) : 0;
  ans += y1 > 0 && z1 > 0 ? Get(x2, y1 - 1, z1 - 1) : 0;
  ans += x1 > 0 && z1 > 0 ? Get(x1 - 1, y2, z1 - 1) : 0;
  ans -= x1 > 0 && y1 > 0 && z1 > 0 ? Get(x1 - 1, y1 - 1, z1 - 1) : 0;
  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int cmd, x, y, z, p, q, r, k;
  std::cin >> n;
  sums = new int[n * n * n];
  for (int i = 0; i < n * n * n; ++i) {
    sums[i] = 0;
  }
  while (true) {
    std::cin >> cmd;
    if (cmd == 1) {
      std::cin >> x >> y >> z >> k;
      Update(x, y, z, k);
    } else if (cmd == 2) {
      std::cin >> x >> y >> z >> p >> q >> r;
      std::cout << Count(x, y, z, p, q, r) << '\n';
    } else {
      break;
    }
  }
  delete[] sums;
}