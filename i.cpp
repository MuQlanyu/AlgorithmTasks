#include <iostream>

int Merge(int* arr, int begin, int end) {
  int med = (begin + end) / 2, indl = 0, indr = 0, cnt = 0;
  int* left = new int[med - begin];
  int* right = new int[end - med];
  for (int i = begin; i < med; ++i) {
    left[i - begin] = arr[i];
  }
  for (int i = med; i < end; ++i) {
    right[i - med] = arr[i];
  }
  while (indl < med - begin && indr < end - med) {
    if (left[indl] < right[indr]) {
      arr[begin + indl + indr] = left[indl];
      ++indl;
      cnt += indr;
    } else {
      arr[begin + indl + indr] = right[indr];
      ++indr;
    }
  }
  while (indl < med - begin) {
    arr[begin + indl + indr] = left[indl];
    ++indl;
    cnt += indr;
  }
  while (indr < end - med) {
    arr[begin + indl + indr] = right[indr];
    ++indr;
  }
  delete[] left;
  delete[] right;
  return cnt;
}

int MergeSort(int* arr, int begin, int end) {
  if (end - begin == 1) {
    return 0;
  }
  int cnt = 0;
  cnt += MergeSort(arr, begin, (begin + end) / 2);
  cnt += MergeSort(arr, (begin + end) / 2, end);
  return cnt + Merge(arr, begin, end);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int n;
  std::cin >> n;
  int* arr = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> arr[i];
  }
  std::cout << MergeSort(arr, 0, n);
  delete[] arr;
}