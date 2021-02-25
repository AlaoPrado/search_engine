/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
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
