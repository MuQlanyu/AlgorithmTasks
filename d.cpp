#include <algorithm>
#include <iostream>
#include <vector>
using std::pair;

bool Cmp(pair<int, int>& left, pair<int, int> right) {
  if (left.second == right.second) {
    return left.first > right.first;
  }
  return left.second < right.second;
}

int F(int x) { return x & (x + 1); }

int G(int x) { return x | (x + 1); }

void Update(long long* sums, int pos, int len) {
  for (int i = pos; i < len; i = G(i)) {
    sums[i] += 1;
  }
}

long long Get(const long long* sums, int pos) {
  long long ans = 0;
  for (int i = pos; i >= 0; i = F(i) - 1) {
    ans += sums[i];
  }
  return ans;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, l, r;
  long long ans = 0, cnt = 1;
  std::cin >> n;
  pair<int, int>* arr = new pair<int, int>[n];
  std::vector<pair<int, int>> coords;
  for (int i = 0; i < n; ++i) {
    std::cin >> l >> r;
    coords.emplace_back(l, -(i + 1));
    coords.emplace_back(r, i + 1);
  }
  l = r = 0;
  std::sort(coords.begin(), coords.end());
  while (l < 2 * n) {
    if (coords[l].second < 0) {
      arr[-coords[l].second - 1].first = r;
    } else {
      arr[coords[l].second - 1].second = r;
    }
    ++l, ++r;
    if (l != 0 && l < 2 * n && coords[l].first == coords[l - 1].first) {
      --r;
    }
  }
  long long* sums = new long long[r + 1];
  for (int i = 0; i <= r; ++i) {
    sums[i] = 0;
  }
  std::sort(arr, arr + n, Cmp);
  for (int i = 1; i < n; ++i) {
    if (arr[i - 1] == arr[i]) {
      ++cnt;
    } else {
      ans -= cnt * (cnt - 1) / 2;
      cnt = 1;
    }
  }
  ans -= cnt * (cnt - 1) / 2;
  for (int i = 0; i < n; ++i) {
    ans += Get(sums, arr[i].second);
    if (arr[i].first != 0) {
      ans -= Get(sums, arr[i].first - 1);
    }
    Update(sums, arr[i].first, r + 1);
  }
  std::cout << ans;
  delete[] sums;
  delete[] arr;
}