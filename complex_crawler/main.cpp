#include "crawler/LongTermCrawler.hpp"
#include "../utils/Assert.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(const int argc, const char **argv) {
  try {
    search_engine::utils::assertTrue(
        argc > 1, "Error: a file name must be passed by parameter");

    const std::string fileName = argv[1];
    std::ifstream seedFile(fileName);

    search_engine::utils::assertTrue(seedFile.is_open(),
                                     "Error: invalid file name");

    std::vector<std::string> seedUrls;
    std::string line;
    while (std::getline(seedFile, line)) {
      seedUrls.push_back(line);
    }

    search_engine::LongTermCrawler crawler;
    crawler.crawl(seedUrls, 1);

  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  return 0;
}
