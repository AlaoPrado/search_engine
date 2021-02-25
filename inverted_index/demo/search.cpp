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
#include "../../utils/Assert.hpp"
#include "../InvertedIndex.hpp"
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main(const int argc, const char **argv) {
  try {
    search_engine::utils::assertTrue(argc > 2,
                                     "Error: an index file name and a search "
                                     "term must be passed by parameter.");

    const std::string indexFileName = argv[1];
    const std::string searchTerm = argv[2];
    std::chrono::steady_clock::time_point start, end;
    std::chrono::milliseconds duration;

    start = std::chrono::steady_clock::now();
    search_engine::InvertedIndex invertedIndex(indexFileName);
    end = std::chrono::steady_clock::now();
    duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Index load time (Millis): " << duration.count() << std::endl;

    std::vector<std::string> urlList;

    invertedIndex.search(searchTerm, urlList);
    if (urlList.empty()) {
      std::cout << "Not found" << std::endl;
    } else {
      for (auto &&url : urlList) {
        std::cout << url << std::endl;
      }
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
