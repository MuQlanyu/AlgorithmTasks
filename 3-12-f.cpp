#include <iostream>
#include <vector>

namespace Constants {
const size_t kMod = 998244353;  // mod = 119 * 2 ^ 23 + 1
const size_t kInitRoot = 31;    // корень из 1 степени 2 ^ 23
const size_t kDeg = 23;         // Сама по себе степень
}  // namespace Constants

// Класс чисто для инкапсуляции
class Fourier {
  std::pair<size_t, size_t> static FindRoots(size_t sz,
                                             std::vector<size_t>& roots) {
    size_t deg = 0;
    size_t num = 1;
    while (sz >= num) {
      ++deg;
      num <<= 1;
    }

    roots.resize(num);
    size_t root = Constants::kInitRoot;
    for (size_t i = 0; i < Constants::kDeg - deg; ++i) {
      root = (root * root) % Constants::kMod;
    }

    roots[0] = 1;
    for (size_t i = 1; i < num; ++i) {
      roots[i] = (roots[i - 1] * root) % Constants::kMod;
    }
    return {deg, num};
  }

  void static CountRev(std::vector<int>& rev, size_t deg, size_t num) {
    int sz = static_cast<int>(num);
    int log = static_cast<int>(deg);
    int res = 0;
    int ind;
    rev[0] = 0;
    for (int cur = 1; cur < sz; ++cur) {
      ind = log - 1;
      while ((res & (1 << ind)) != 0) {
        res ^= (1 << ind--);
      }
      res |= (1 << ind);
      rev[cur] = res;
    }
  }

  void static InitValues(std::vector<int>& rev, std::vector<size_t>& roots,
                         std::vector<size_t>& values) {
    for (size_t i = 0; i < roots.size(); ++i) {
      values[i] = roots[rev[i]];
    }
  }

  size_t static FastPow(size_t num, size_t pow) {
    num %= Constants::kMod;
    if (pow == 1) {
      return num % Constants::kMod;
    }
    if (pow % 2 == 0) {
      return FastPow(num * num, pow / 2) % Constants::kMod;
    }
    return (num * FastPow(num * num, pow / 2)) % Constants::kMod;
  }

  void static FFT(std::vector<size_t>& values, std::vector<size_t>& roots,
                  bool straight) {
    std::pair<size_t, size_t> tmp;
    size_t cur_sz = roots.size();
    size_t shift;
    size_t step;
    for (size_t len = 2; len <= roots.size(); len <<= 1) {
      cur_sz /= 2;
      shift = (straight ? cur_sz : roots.size() - cur_sz);
      for (size_t i = 0; i < values.size(); i += len) {
        step = 0;
        for (size_t j = 0; j < len / 2; ++j) {
          tmp.first = values[j + i];
          tmp.second = roots[step] * values[j + i + len / 2] % Constants::kMod;
          values[j + i] = (tmp.first + tmp.second) % Constants::kMod;
          values[j + i + len / 2] =
              (tmp.first + Constants::kMod - tmp.second) % Constants::kMod;
          step += shift;
          if (step >= roots.size()) {
            step -= roots.size();
          }
        }
      }
    }

    if (!straight) {
      size_t back = FastPow(values.size(), Constants::kMod - 2);
      for (size_t& value : values) {
        value = (value * back) % Constants::kMod;
      }
    }
  }

  void static PrintResult(size_t init_len, std::vector<size_t>& values) {
    std::cout << init_len << ' ';
    for (size_t i = 0; i <= init_len; ++i) {
      if (values[init_len - i] > Constants::kMod / 2) {
        std::cout << static_cast<long long>(values[init_len - i]) -
                         static_cast<long long>(Constants::kMod);
      } else {
        std::cout << values[init_len - i];
      }
      std::cout << ' ';
    }
  }

 public:
  void static Multiply(std::vector<size_t>& left, std::vector<size_t>& right) {
    size_t init_len = left.size() + right.size() - 2;

    std::vector<size_t> roots;
    std::pair<size_t, size_t> len =
        FindRoots(left.size() + right.size(), roots);
    left.resize(len.second, 0);
    right.resize(len.second, 0);

    std::vector<int> rev(len.second);
    CountRev(rev, len.first, len.second);

    std::vector<size_t> values_left(len.second);
    InitValues(rev, left, values_left);
    std::vector<size_t> values_right(len.second);
    InitValues(rev, right, values_right);

    FFT(values_left, roots, true);
    FFT(values_right, roots, true);
    for (size_t i = 0; i < len.second; ++i) {
      values_left[i] = (values_left[i] * values_right[i]) % Constants::kMod;
    }

    std::vector<size_t> values(len.second);
    InitValues(rev, values_left, values);

    FFT(values, roots, false);
    PrintResult(init_len, values);
  }
};

void FillPoly(std::vector<size_t>& poly) {
  size_t num;
  std::cin >> num;
  int tmp;
  poly.resize(num + 1);
  for (size_t i = 0; i <= num; ++i) {
    std::cin >> tmp;
    poly[num - i] =
        static_cast<size_t>((tmp >= 0 ? tmp : Constants::kMod + tmp));
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::vector<size_t> first_poly;
  std::vector<size_t> second_poly;

  FillPoly(first_poly);
  FillPoly(second_poly);

  Fourier::Multiply(first_poly, second_poly);
}