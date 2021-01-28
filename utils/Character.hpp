#pragma once

namespace search_engine {

namespace utils {

bool inline isSpacing(const char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool inline isMisc(const char c) {
  return (c >= '!' && c <= '/') || (c >= ':' && c <= '@') ||
         (c >= '[' && c <= '`') || (c >= '{') || (c < 31 && !isSpacing(c));
}

} // namespace utils
} // namespace search_engine
