#include <iostream>
#include <vector>
using std::pair;
using std::string;
using std::vector;
int mod = 1;

bool IsOne(string& str) { return str.size() == 1 && str[0] == '1'; }

bool IsEven(string& str) { return static_cast<int>(str[0]) % 2 == 0; }

string& Minus(string& str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (str[i] != '0') {
      str[i] = static_cast<char>(static_cast<int>(str[i]) - 1);
      if (str[i] == '0') {
        for (size_t j = 0; j < i; ++j) {
          str[j] = '9';
        }
        int ind = str.size() - 1;
        while (str[ind] == '0') {
          --ind;
        }
        str.erase(ind + 1, str.size());
        return str;
      }
      return str;
    }
  }
  return str;
}

string& Half(string& str) {
  int elem = 0, ind = str.size() - 1;
  bool carry = false;
  for (size_t i = str.size(); i > 0; --i) {
    elem = static_cast<int>(str[i - 1]) - 48;
    str[i - 1] = static_cast<char>((elem + (carry ? 10 : 0)) / 2 + 48);
    carry = elem % 2 == 1;
  }
  while (str[ind] == '0') {
    --ind;
  }
  str.erase(ind + 1, str.size());
  return str;
}

bool Bit(int mask, int i) { return ((mask >> i) & 1) != 0; }

int RemoveBit(int mask, int i) { return mask ^ (1 << i); }

int AddBit(int mask, int i) { return mask | (1 << i); }

class Matrix {
 public:
  Matrix(int n) : size_(n) {
    arr_.resize(n);
    for (int i = 0; i < n; ++i) {
      arr_[i].resize(n);
    }
  }

  vector<int>& operator[](int index) { return arr_[index]; }
  const vector<int>& operator[](int index) const { return arr_[index]; }
  int Size() const { return size_; }

 private:
  vector<vector<int>> arr_;
  int size_ = 0;
};

Matrix operator*(const Matrix& left, const Matrix& right) {
  int elem = 0, n = left.Size();
  Matrix res(n);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      elem = 0;
      for (int k = 0; k < n; ++k) {
        elem = (elem + left[i][k] * right[k][j]) % mod;
      }
      res[i][j] = elem % mod;
    }
  }
  return res;
}

bool Check(int left, int right, int n) {
  for (int i = 1; i < n; ++i) {
    if (Bit(left, i) == Bit(right, i)) {
      if (Bit(left, i - 1) == Bit(right, i - 1)) {
        if (Bit(left, i - 1) == Bit(left, i)) {
          return false;
        }
      }
    }
  }
  return true;
}

Matrix BinPow(Matrix matrix, string& pow) {
  if (IsOne(pow)) {
    return matrix;
  }
  if (IsEven(pow)) {
    return BinPow(matrix * matrix, Half(pow));
  }
  return BinPow(matrix * matrix, Half(pow)) * matrix;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  string n;
  int m, ans = 0;
  std::cin >> n >> m >> mod;
  int size = 1 << m;
  for (size_t i = 0; i < n.size() / 2; ++i) {
    std::swap(n[i], n[n.size() - 1 - i]);
  }
  if (IsOne(n)) {
    std::cout << size % mod;
    return 0;
  }

  Matrix res(size);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      res[i][j] = res[j][i] = (Check(i, j, m) ? 1 : 0);
    }
  }
  Matrix ans_matrix = BinPow(res, Minus(n));
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      ans = (ans + ans_matrix[i][j]) % mod;
    }
  }
  std::cout << ans % mod;
}