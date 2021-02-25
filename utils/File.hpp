#pragma once

#include "Assert.hpp"
#include <cstddef>
#include <fstream>
#include <string>

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
