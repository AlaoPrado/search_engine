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
  this->siteAttributesMap = new std::map<std::string, SiteAttributes>();
  this->lastCrawlEndTimeMap = new std::map<std::string, Crawl::timePoint>();
}

Crawler::~Crawler() {
  delete this->viewedUrls;
  delete this->siteAttributesMap;
  delete this->lastCrawlEndTimeMap;
}

void Crawler::printCrawlStatus() {
  if (this->verbose) {
    for (auto it = this->siteAttributesMap->begin();
         it != this->siteAttributesMap->end(); it++) {
      std::cout << "Web site: " << it->first << std::endl;
      std::cout << "Number of URLs at level 1 crawled: "
                << it->second.getNumPagesLeve1() << std::endl;
      std::cout << "Average crawl time for (milliseconds): "
                << it->second.getAverageTime() << std::endl;
      std::cout << "Average page size (Bytes): "
                << it->second.getAveragePageSize() << std::endl;
      std::cout << std::endl;
    }
  }
}

} // namespace search_engine
