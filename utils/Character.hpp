#pragma once

namespace search_engine {

namespace utils {

bool inline isAccentuation(const char c) {
  return c == '^' || c == '`' || c == '~';
}

bool inline isLetter(const char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool inline isMisc(const char c) {
  return (c >= '!' && c <= '/') || (c >= ':' && c <= '@') ||
         (c >= '[' && c <= ']') || (c == '_') || (c >= '{' && c <= '}');
}

bool inline isSpacing(const char c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}


} // namespace utils

} // namespace search_engine
