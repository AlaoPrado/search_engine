#pragma once

namespace search_engine {

namespace utils {

unsigned short charUtf8Char2ToUShort(unsigned char c1, unsigned char c2);

bool isCharUtf8Latin(unsigned short c);

bool isCharUtf8LatinMisc(unsigned short c);

int getCharUtf8NumBytes(const unsigned char c);

} // namespace utils
} // namespace search_engine
