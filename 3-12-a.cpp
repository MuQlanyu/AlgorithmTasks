#include <iostream>
#include <vector>

namespace Constants {
const int kNumNumber = 4;
const long long kMod = 1e9 + 7;
}  // namespace Constants

long long FastPow(long long num, long long pow) {
  num %= Constants::kMod;
  if (pow == 1) {
    return num % Constants::kMod;
  }
  if (pow % 2 == 0) {
    return FastPow(num * num, pow / 2) % Constants::kMod;
  }
  return (num * FastPow(num * num, pow / 2)) % Constants::kMod;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::vector<long long> num(Constants::kNumNumber);
  for (int i = 0; i < Constants::kNumNumber; ++i) {
    std::cin >> num[i];
    num[i] = (num[i] + Constants::kMod) % Constants::kMod;
  }

  long long ans = (num[0] * num[3] + num[1] * num[2]) % Constants::kMod;
  ans = ans * FastPow(num[1] * num[3], Constants::kMod - 2);
  std::cout << ans % Constants::kMod;
}