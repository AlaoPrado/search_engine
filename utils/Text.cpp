#include "Text.hpp"
#include <cctype>

namespace search_engine {
namespace utils {

std::string textToLowerCase(std::string text) {
  for (auto &&letter : text) {
    letter = std::tolower(letter);
  }

  return text;
}

} // namespace utils
} // namespace search_engine
