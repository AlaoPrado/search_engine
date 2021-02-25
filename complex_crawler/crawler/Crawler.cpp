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
                << it->second.getNumPagesLevel1() << std::endl;
      std::cout << "Average crawl time for (milliseconds): "
                << it->second.getAverageTime() << std::endl;
      std::cout << "Average page size (Bytes): "
                << it->second.getAveragePageSize() << std::endl;
      std::cout << std::endl;
    }
  }
}

} // namespace search_engine
