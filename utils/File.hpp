#pragma once

#include "Assert.hpp"
#include <cstddef>
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

std::size_t inline fileSize(const std::string directory) {
  std::ifstream file(directory, std::ifstream::binary);

  utils::assertTrue(file.is_open(),
                    "Error(utils/fileSize): failed to open file");

  file.seekg(0, file.end);

  std::size_t numBytes = file.tellg();

  file.close();

  return numBytes;
}

} // namespace utils
} // namespace search_engine
