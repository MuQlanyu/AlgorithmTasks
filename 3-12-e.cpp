#include <iostream>
#include <random>
#include <vector>

namespace Random {
const size_t kMaxRand = 1e9 + 7;
std::random_device dev;
std::mt19937 mt(dev());
std::uniform_int_distribution<std::mt19937::result_type> rnd(0, kMaxRand);
}  // namespace Random

size_t GetRandom() { return Random::rnd(Random::mt); }

class Field {
  size_t real_;
  size_t imag_;
  size_t root_;
  size_t prime_;

 public:
  Field(size_t prime, size_t real, size_t imag = 0, size_t root = 0)
      : real_(real), imag_(imag), root_(root), prime_(prime){};

  Field& operator*=(const Field& other) {
    Field tmp = *this;
    size_t tmp_num = ((tmp.imag_ * other.imag_) % prime_) * tmp.root_;
    real_ = (tmp.real_ * other.real_ + tmp_num) % prime_;
    imag_ = (tmp.real_ * other.imag_ + tmp.imag_ * other.real_) % prime_;
    return *this;
  }

  Field& operator%=(size_t mod) {
    real_ %= mod;
    imag_ %= mod;
    return *this;
  }

  size_t GetReal() const { return real_; }
};

Field operator*(Field first, const Field& other) { return first *= other; }
Field operator%(Field field, size_t mod) { return field %= mod; }

template <class T>
T FastPow(T num, size_t pow, size_t prime) {
  num %= prime;
  if (pow == 1) {
    return num % prime;
  }
  if (pow % 2 == 0) {
    return FastPow<T>(num * num, pow / 2, prime) % prime;
  }
  return (num * FastPow<T>(num * num, pow / 2, prime)) % prime;
}

bool ProcessIrregular(size_t num, size_t prime) {
  if (num != 0 && prime >= 3 &&
      FastPow<size_t>(num, (prime - 1) / 2, prime) == 1) {
    return false;
  }
  if (num == 0) {
    std::cout << 0 << '\n';
  } else {
    std::cout << (prime <= 2 ? "1\n" : "IMPOSSIBLE\n");
  }
  return true;
}

size_t FindNotSquare(size_t num, size_t prime) {
  size_t ans = num;
  size_t root = (ans * ans - num + prime) % prime;
  while (root == 0 || FastPow<size_t>(root, (prime - 1) / 2, prime) == 1) {
    ans = GetRandom() % prime;
    root = (ans * ans - num + prime) % prime;
  }
  return ans;
}

void FindSquareRoot(size_t num, size_t prime) {
  if (ProcessIrregular(num, prime)) {
    return;
  }

  size_t not_square = FindNotSquare(num, prime);
  size_t tmp = (not_square * not_square - num + prime) % prime;
  Field root(prime, not_square, 1, tmp);
  std::cout << FastPow<Field>(root, (prime + 1) / 2, prime).GetReal() << '\n';
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  size_t test_count;
  size_t num;
  size_t prime;

  std::cin >> test_count;
  for (size_t i = 0; i < test_count; ++i) {
    std::cin >> num >> prime;
    FindSquareRoot(num, prime);
  }
}