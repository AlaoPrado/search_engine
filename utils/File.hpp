#pragma once

#include "Assert.hpp"
#include <fstream>
#include <string>

namespace search_engine {
namespace utils {

void inline fileWrite(const std::string directory, const std::string text) {
  std::ofstream file(directory, std::ofstream::out);

  utils::assertTrue(file.is_open(),
                    "Error(utils/fileWrite): failed to open file");

  file << text;
  file.close();
}

} // namespace utils
} // namespace search_engine
