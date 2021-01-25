#include "TextUtf8.hpp"
#include <CkString.h>
#include <cctype>

namespace search_engine {
namespace utils {

std::string textToLowerCaseUtf8(std::string text) {
  CkString textUtf8;
  textUtf8.appendUtf8(text.c_str());
  textUtf8.toLowerCase();
  std::string textUtf8Lowercase(textUtf8.getStringUtf8());
  return textUtf8Lowercase;
}

} // namespace utils
} // namespace search_engine
