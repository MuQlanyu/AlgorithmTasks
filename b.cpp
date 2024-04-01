#include <iostream>
#include <vector>
using std::pair;
using std::vector;
long long mod = 1e9 + 9;

class Matrix {
  vector<vector<long long>> arr_;
  long long n_ = 0;
  long long m_ = 0;

 public:
  Matrix(long long n, long long m) : n_(n), m_(m) {
    arr_.resize(n);
    for (long long i = 0; i < n; ++i) {
      arr_[i].assign(m, 0);
    }
  }

  vector<long long>& operator[](long long index) { return arr_[index]; }
  const vector<long long>& operator[](long long index) const {
    return arr_[index];
  }
  pair<long long, long long> Size() const { return {n_, m_}; }
};

Matrix operator*(const Matrix& left, const Matrix& right) {
  long long elem = 0;
  Matrix res(left.Size().first, right.Size().second);
  for (long long i = 0; i < left.Size().first; ++i) {
    for (long long j = 0; j < right.Size().second; ++j) {
      elem = 0;
      for (long long k = 0; k < left.Size().second; ++k) {
        elem = (elem + left[i][k] * right[k][j]) % mod;
      }
      res[i][j] = elem % mod;
    }
  }
  return res;
}

Matrix BinPow(Matrix matrix, long long pow) {
  if (pow == 1) {
    return matrix;
  }
  if (pow % 2 == 0) {
    return BinPow(matrix * matrix, pow / 2);
  }
  return BinPow(matrix * matrix, pow / 2) * matrix;
}

long long Fib(long long n) {
  if (n == 0) {
    return 1ll;
  }
  Matrix fib(2, 1), multi(2, 2);
  fib[0] = {1};
  fib[1] = {0};
  multi[0] = {1, 1};
  multi[1] = {1, 0};
  return (BinPow(multi, n) * fib)[0][0];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  long long n, q, diag, ans, tmp;
  std::cin >> n >> q;
  vector<long long> num(n);
  vector<pair<long long, long long>> coord(n);
  for (long long i = 0; i < n; ++i) {
    std::cin >> coord[i].first >> coord[i].second >> tmp;
    num[i] = tmp % mod;
  }
  for (long long i = 0; i < q; ++i) {
    std::cin >> diag;
    ans = 0;
    for (long long j = 0; j < n; ++j) {
      tmp = coord[j].second - coord[j].first - diag;
      if (tmp >= 0) {
        ans = (ans + num[j] * Fib(tmp)) % mod;
      }
    }
    std::cout << ans << '\n';
  }
}