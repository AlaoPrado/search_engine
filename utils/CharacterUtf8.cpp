#include "CharacterUtf8.hpp"

namespace search_engine {

namespace utils {

unsigned short charUtf8Char2ToUShort(unsigned char c1, unsigned char c2) {
  return ((unsigned short)c1 << 8) + c2;
}

bool isCharUtf8Latin(unsigned short c) { return c <= 0xcaaf; }

bool isCharUtf8LatinMisc(unsigned short c) {
  return (c >= 0xc280 && c <= 0xc2bf) || c == 0xc397 || c == 0xc3b7;
}

int getCharUtf8NumBytes(const unsigned char c) {
  if (c < 128 + 64) {
    return 1;
  }

  if (c < 128 + 64 + 32) {
    return 2;
  }

  if (c < 128 + 64 + 32 + 16) {
    return 3;
  }

  return 4;
}

} // namespace utils
} // namespace search_engine
