#include "../utils/Assert.hpp"
#include "InvertedIndex.hpp"
#include <cstddef>
#include <iostream>
#include <string>

int main(const int argc, const char **argv) {
  try {
    search_engine::utils::assertTrue(
        argc > 1,
        "Error: an collection directory must be passed by parameter.");

    const std::string collectionDirectory = argv[1];
    search_engine::InvertedIndex invertedIndex(collectionDirectory);

  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
