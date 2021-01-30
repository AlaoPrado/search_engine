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
    search_engine::InvertedIndex invertedIndex(indexFileName);
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
