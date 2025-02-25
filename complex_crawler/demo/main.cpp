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
#include "../crawler/Crawler.hpp"
#include "../crawler/GeneralShortTermCrawler.hpp"
#include "../crawler/LongTermCrawler.hpp"
#include "../crawler/ShortTermCrawler.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(const int argc, const char **argv) {
  search_engine::Crawler *crawler = NULL;

  try {
    const int LONG_TERM_CRAWLER = 0;
    const int SHORT_TERM_CRAWLER = 1;
    const int G_SHORT_TERM_CRAWLER = 2;

    search_engine::utils::assertTrue(
        argc > 1,
        "Error: a file name and a storage folder must be passed by parameter");

    const std::string fileName = argv[1];
    const std::string storageFolderDirectory = argc > 2 ? argv[2] : "storage/";
    const int crawlerType = argc > 3 ? std::stoi(argv[3]) : LONG_TERM_CRAWLER;

    search_engine::utils::assertTrue(
        crawlerType == LONG_TERM_CRAWLER || crawlerType == SHORT_TERM_CRAWLER ||
            crawlerType == G_SHORT_TERM_CRAWLER,
        "Error: the third parameter must be 0 (long term crawler), 1 (short "
        "term crawler) or 2 (general short term crawler)");

    const std::size_t numPagesToCrawl =
        (std::size_t)(argc > 4 ? std::stoi(argv[4]) : 10);
    const int numThreads = argc > 5 ? std::stoi(argv[5]) : 1;

    search_engine::utils::assertTrue(
        numThreads > 0,
        "Error: the fifth parameter must valid number of threads");

    std::ifstream seedFile(fileName);

    search_engine::utils::assertTrue(seedFile.is_open(),
                                     "Error: invalid input file name");

    std::vector<std::string> seedUrls;
    std::string line;
    while (std::getline(seedFile, line)) {
      seedUrls.push_back(line);
    }
    seedFile.close();

    if (crawlerType == LONG_TERM_CRAWLER) {
      crawler = (search_engine::Crawler *)new search_engine::LongTermCrawler(
          storageFolderDirectory);
    } else if (crawlerType == SHORT_TERM_CRAWLER) {
      crawler = (search_engine::Crawler *)new search_engine::ShortTermCrawler(
          storageFolderDirectory, true, numThreads);
    } else if (crawlerType == G_SHORT_TERM_CRAWLER) {
      crawler =
          (search_engine::Crawler *)new search_engine::GeneralShortTermCrawler(
              storageFolderDirectory, true, numThreads);
    }

    crawler->crawl(seedUrls, numPagesToCrawl);

    delete crawler;
  } catch (std::exception &e) {
    if (crawler != NULL) {
      delete crawler;
    }

    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
