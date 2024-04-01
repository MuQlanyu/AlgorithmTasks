#include <iostream>
#include <vector>

namespace Constants {
const size_t kMod = 998244353;  // mod = 119 * 2 ^ 23 + 1
const size_t kInitRoot = 31;    // корень из 1 степени 2 ^ 23
const size_t kDeg = 23;         // Сама по себе степень
const size_t kSeven = 7;
const long long kGreatSeven = kMod * kSeven;
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

  void static ReturnValues(std::vector<size_t>& poly) {
    long long tmp;
    for (size_t& elem : poly) {
      if (elem > Constants::kMod / 2) {
        tmp = static_cast<long long>(elem);
        tmp = tmp - static_cast<long long>(Constants::kMod) +
              Constants::kGreatSeven;
        elem = static_cast<size_t>(tmp) % Constants::kSeven;
      } else {
        elem %= Constants::kSeven;
      }
    }
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

  std::vector<size_t> static Slice(const std::vector<size_t>& poly,
                                   size_t slice) {
    std::vector<size_t> res(poly.size() - slice);
    for (size_t i = 0; i < poly.size() - slice; ++i) {
      res[i] = poly[slice + i];
    }
    return res;
  }

  void static Append(std::vector<size_t>& to, std::vector<size_t>& from,
                     size_t size) {
    size_t sz = to.size();
    if (from.size() < size) {
      size = from.size();
    }
    to.resize(sz + size);
    for (size_t i = 0; i < size; ++i) {
      to[sz + i] = from[i];
    }
  }

  void static ReversePoly(std::vector<size_t>& poly) {
    if (poly.empty()) {
      return;
    }
    size_t left = 0;
    size_t right = poly.size() - 1;
    while (left < right) {
      std::swap(poly[left], poly[right]);
      ++left;
      --right;
    }
  }

  void static PrintPoly(std::vector<size_t>& poly) {
    if (poly.empty()) {
      std::cout << "0 0\n";
      return;
    }
    ReturnValues(poly);

    size_t oldest = poly.size() - 1;
    for (; oldest != 0; --oldest) {
      if (poly[oldest] != 0) {
        break;
      }
    }
    std::cout << oldest;
    for (int i = static_cast<int>(oldest); i >= 0; --i) {
      std::cout << ' ' << poly[i];
    }
    std::cout << '\n';
  }

  bool static ProcessLess(std::vector<size_t>& left,
                          std::vector<size_t>& right) {
    if (left.size() < right.size()) {
      std::cout << "0 0\n";
      PrintPoly(left);
    }
    return left.size() < right.size();
  }

 public:
  std::vector<size_t> static Multiply(std::vector<size_t>& left,
                                      std::vector<size_t>& right,
                                      size_t init_len) {
    std::pair<size_t, size_t> sizes = {left.size(), right.size()};

    std::vector<size_t> roots;
    std::pair<size_t, size_t> len = FindRoots(init_len, roots);
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

    left.resize(sizes.first);
    right.resize(sizes.second);
    return values;
  }

  std::vector<size_t> static ModDivision(std::vector<size_t>& poly,
                                         size_t mod) {
    if (poly[0] == 0) {
      return {};
    }

    std::vector<size_t> res(1, 0);
    res[0] = FastPow(poly[0], Constants::kSeven - 2) % Constants::kSeven;
    size_t cur = 1;

    std::vector<size_t> poly0(1, poly[0]);
    std::vector<size_t> poly1;

    std::vector<size_t> res_t(1, res[0]);
    std::vector<size_t> rest(1, 0);

    std::vector<size_t> tmp;
    size_t sz;

    while (cur < mod) {
      poly1 = Slice(poly, cur);
      rest = Slice(Multiply(poly0, res, cur + cur), cur);
      ReturnValues(rest);

      tmp = Multiply(res, poly1, cur + poly1.size());
      sz = (rest.size() > tmp.size() ? rest.size() : tmp.size());
      if (sz > mod) {
        sz = mod;
      }
      tmp.resize(sz, 0);
      for (size_t i = 0; i < sz; ++i) {
        tmp[i] = Constants::kMod - tmp[i];
        if (i < rest.size()) {
          tmp[i] = tmp[i] + Constants::kMod - rest[i];
        }
        tmp[i] %= Constants::kMod;
      }
      ReturnValues(tmp);

      res_t = Multiply(res, tmp, sz + cur);
      res_t.resize(cur);
      ReturnValues(res_t);

      Append(poly0, poly1, cur);
      Append(res, res_t, cur);
      cur *= 2;
    }
    res.resize(mod);
    return res;
  }

  void static Division(std::vector<size_t>& left, std::vector<size_t>& right) {
    if (ProcessLess(left, right)) {
      return;
    }
    ReversePoly(left);
    ReversePoly(right);

    size_t mod = left.size() - right.size() + 1;
    size_t right_init_sz = right.size();
    if (mod > right_init_sz) {
      right.resize(mod, 0);
    }
    std::vector<size_t> remainder = ModDivision(right, mod);
    std::vector<size_t> quotient =
        Multiply(left, remainder, left.size() + mod - 1);
    quotient.resize(mod);
    right.resize(right_init_sz);
    ReturnValues(remainder);
    ReturnValues(quotient);

    ReversePoly(quotient);
    ReversePoly(left);
    ReversePoly(right);

    remainder = Multiply(quotient, right, right.size() + mod - 1);
    if (remainder.size() < left.size()) {
      remainder.resize(left.size(), 0);
    }

    for (size_t i = 0; i < remainder.size(); ++i) {
      remainder[i] = Constants::kMod - remainder[i];
      if (i < left.size()) {
        remainder[i] += left[i];
      }
      remainder[i] %= Constants::kMod;
    }
    remainder.resize(right.size() - 1, 0);

    PrintPoly(quotient);
    PrintPoly(remainder);
  }
};

void FillPoly(std::vector<size_t>& poly) {
  size_t num;
  std::cin >> num;

  poly.resize(num + 1);
  for (size_t i = 0; i <= num; ++i) {
    std::cin >> poly[num - i];
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  std::vector<size_t> first;
  std::vector<size_t> second;

  FillPoly(first);
  FillPoly(second);

  Fourier::Division(first, second);
}