#include "Crawler.hpp"
#include "../../utils/Assert.hpp"
#include "../../utils/Url.hpp"
#include <CkSpider.h>
#include <exception>
#include <iostream>
#include <map>
#include <string>

namespace search_engine {

Crawler::Crawler(std::string storageDirectory, bool verbose)
    : storageDirectory(storageDirectory), verbose(verbose) {
  this->mustMatchPatterns.push_back("*.br*");
  this->avoidPatterns.push_back("*.pdf");
  this->viewedUrls = new std::map<std::string, bool>();
}

Crawler::~Crawler() { delete this->viewedUrls; }

} // namespace search_engine
