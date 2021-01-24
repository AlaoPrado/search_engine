#pragma once

#include <cstddef>
#include <string>

namespace search_engine {

class Occurrence {
private:
  std::string word;
  std::size_t position;

public:
  Occurrence(std::string word, std::size_t position);
  std::string getWord();
  std::size_t getPosition();
};

} // namespace search_engine
