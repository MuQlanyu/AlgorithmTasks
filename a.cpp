#include <iostream>
using std::pair;

void SwapPair(int* arr, int* r, int i, int j) {
  std::swap(arr[i], arr[j]);
  std::swap(r[i], r[j]);
}

std::pair<int, int> Partition(int* arr, int* r, int begin, int end, int pivot) {
  int indl = begin;
  for (int i = begin; i < end; ++i) {
    if (arr[i] < pivot) {
      SwapPair(arr, r, i, indl++);
    }
  }
  int indr = indl;
  for (int i = indl; i < end; ++i) {
    if (arr[i] == pivot) {
      SwapPair(arr, r, i, indr++);
    }
  }
  return std::make_pair(indl, indr - 1);
}

int FindMed(int* arr, int* r, int begin, int end) {
  int min, med = (begin + end + 1) / 2;
  for (int i = begin; i < med; ++i) {
    min = i;
    for (int m = i + 1; m < end; ++m) {
      if (arr[m] < arr[min]) {
        min = m;
      }
    }
    SwapPair(arr, r, i, min);
  }
  return arr[med - 1];
}

int FindStatistic(int* arr, int* r, int begin, int end, int k) {
  if (end - begin <= 1) {
    return arr[begin];
  }
  int len = end - begin, med_len = (len + 4) / 5;
  int* b = new int[med_len];
  for (int i = begin; i < end; i += 5) {
    b[(i - begin) / 5] = FindMed(arr, r, i, std::min(i + 5, end));
  }
  int pivot = FindStatistic(b, b, 0, med_len, med_len / 2);
  std::pair<int, int> med = Partition(arr, r, begin, end, pivot);
  delete[] b;
  if (k < med.first) {
    return FindStatistic(arr, r, begin, med.first, k);
  }
  if (k <= med.second) {
    return arr[med.second];
  }
  return FindStatistic(arr, r, med.second + 1, end, k);
}

void QuickSort(int* arr, int* r, int begin, int end) {
  if (end - begin < 1) {
    return;
  }
  int med_len = (end - begin + 4) / 5;
  int* b = new int[med_len];
  for (int i = begin; i < end; i += 5) {
    b[(i - begin) / 5] = FindMed(arr, r, i, std::min(i + 5, end));
  }
  int pivot = FindStatistic(b, b, 0, med_len, med_len / 2);
  delete[] b;
  std::pair<int, int> med = Partition(arr, r, begin, end, pivot);
  QuickSort(arr, r, begin, med.first);
  QuickSort(arr, r, med.second + 1, end);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  int n, l, r, cnt = 0, begin = 0, end = 0;
  std::cin >> n;
  int* first = new int[n];
  int* second = new int[n];
  for (int i = 0; i < n; ++i) {
    std::cin >> l >> r;
    first[i] = l;
    second[i] = r;
  }
  QuickSort(first, second, 0, n);
  pair<int, int>* ans = new pair<int, int>[n];
  begin = first[0];
  end = second[0];
  for (int i = 1; i < n; ++i) {
    if (first[i] > end) {
      ans[cnt++] = std::make_pair(begin, end);
      begin = first[i];
      end = second[i];
    } else {
      end = std::max(end, second[i]);
    }
  }
  delete[] first;
  delete[] second;
  ans[cnt++] = std::make_pair(begin, end);
  std::cout << cnt;
  for (int i = 0; i < cnt; ++i) {
    std::cout << '\n' << ans[i].first << ' ' << ans[i].second;
  }
  delete[] ans;
}
