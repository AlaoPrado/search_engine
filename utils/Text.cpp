#include "Text.hpp"
#include <cctype>

namespace search_engine {
namespace utils {

void textToLowerCase(std::string &text) {
  for (auto &&letter : text) {
    letter = std::tolower(letter);
  }
}

} // namespace utils
} // namespace search_engine
