// no_concepts
#include <iostream>
#include <unordered_map>
#include <vector>

using Vec = std::vector<std::vector<long long>>;

namespace MagicNumbers {
const std::vector<long long> kMod = {433494437, 303595777};
const std::vector<long long> kMult = {29, 31};
const long long kSize = 3e6 + 10;
}  // namespace MagicNumbers

void MakePref(Vec& pref, const std::string& str, Vec& pow) {
  pref.resize(2, std::vector<long long>(str.size()));
  pref[0][0] = pref[1][0] = static_cast<unsigned char>(str[0] - 'a' + 1);
  for (int mod = 0; mod < 2; ++mod) {
    for (int i = 1; i < static_cast<int>(str.size()); ++i) {
      pref[mod][i] = (pref[mod][i - 1] + (str[i] - 'a' + 1) * pow[mod][i]) %
                     MagicNumbers::kMod[mod];
    }
  }
}

std::pair<long long, long long> Get(int left, int right, Vec& pref, Vec& pow) {
  if (left == 0) {
    return {pref[0][right] * pow[0][MagicNumbers::kSize - right] %
                MagicNumbers::kMod[0],
            pref[1][right] * pow[1][MagicNumbers::kSize - right] %
                MagicNumbers::kMod[1]};
  }
  return {(pref[0][right] - pref[0][left - 1] + MagicNumbers::kMod[0]) *
              pow[0][MagicNumbers::kSize - right] % MagicNumbers::kMod[0],
          (pref[1][right] - pref[1][left - 1] + MagicNumbers::kMod[1]) *
              pow[1][MagicNumbers::kSize - right] % MagicNumbers::kMod[1]};
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  Vec pow(2, std::vector<long long>(MagicNumbers::kSize + 1));
  pow[0][0] = pow[1][0] = 1;
  for (int ind = 1; ind <= MagicNumbers::kSize; ++ind) {
    pow[0][ind] =
        (pow[0][ind - 1] * MagicNumbers::kMult[0]) % MagicNumbers::kMod[0];
    pow[1][ind] =
        (pow[1][ind - 1] * MagicNumbers::kMult[1]) % MagicNumbers::kMod[1];
  }

  std::string pattern;
  std::cin >> pattern;
  Vec pref_pattern;
  MakePref(pref_pattern, pattern + pattern, pow);

  std::string text;
  std::cin >> text;
  Vec pref_text;
  MakePref(pref_text, text, pow);

  std::unordered_map<long long, long long> first;
  first.reserve(2 * pattern.size());
  int sz = static_cast<int>(pattern.size());
  std::pair<long long, long long> hash;
  for (int i = 0; i < sz; ++i) {
    hash = Get(i, i + sz - 1, pref_pattern, pow);
    first[hash.first] = hash.second;
  }

  auto it_first = first.end();
  int ans = 0;
  for (int i = 0; i < static_cast<int>(text.size()) - sz + 1; ++i) {
    hash = Get(i, i + sz - 1, pref_text, pow);
    it_first = first.find(hash.first);
    if (it_first != first.end() && it_first->second == hash.second) {
      ++ans;
    }
  }

  std::cout << ans;
}