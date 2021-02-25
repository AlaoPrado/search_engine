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
#include "LongTermCrawler.hpp"
#include "../../utils/Url.hpp"
#include "SiteAttributes.hpp"
#include "action/Crawl.hpp"
#include "action/PageStorage.hpp"
#include "action/PopFromScheduler.hpp"
#include "action/PushIntoScheduler.hpp"
#include "scheduler/PriorityPageScheduler.hpp"
#include <CkSpider.h>
#include <chrono>
#include <exception>
#include <iostream>
namespace search_engine {

LongTermCrawler::LongTermCrawler(std::string storageDirectory, bool verbose)
    : Crawler(storageDirectory, verbose) {}

LongTermCrawler::~LongTermCrawler() {}

void LongTermCrawler::crawl(std::vector<std::string> &seedUrls,
                            std::size_t numPagesToCrawl) {
  auto *pageScheduler = new PriorityPageScheduler();

  int numPagesPushed;
  int numFails = 0;
  PushIntoScheduler::push(*pageScheduler, seedUrls, *(this->viewedUrls),
                          numPagesToCrawl, numPagesPushed);

  std::size_t numCrawledPages = 0;
  while (numCrawledPages < numPagesToCrawl) {
    Page page("");
    std::string baseUrl;
    bool useLastCrawlEndTime;

    PopFromScheduler::pop(*pageScheduler, page, baseUrl, useLastCrawlEndTime,
                          *siteAttributesMap, *lastCrawlEndTimeMap);

    CkSpider spider;
    SiteAttributes *siteAttributes = &(siteAttributesMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    try {
      Crawl::crawlUrl(spider, page, this->mustMatchPatterns,
                      this->avoidPatterns, *siteAttributes, *lastCrawlEndTime,
                      useLastCrawlEndTime);

      PageStorage::storePage(this->storageDirectory, spider, numCrawledPages);
      numCrawledPages++;

      PushIntoScheduler::push(*pageScheduler, spider, *(this->viewedUrls),
                              numPagesToCrawl + numFails, page.getLevel(),
                              numPagesPushed);

      if (page.getLevel() == 0) {
        siteAttributes->addNumPagesLevel1(numPagesPushed);
      }
    } catch (std::exception &e) {
      std::cout << "Error while crawling page " + page.getUrl() << std::endl;
      std::cout << e.what() << std::endl;
      numFails++;
    }
  }

  this->printCrawlStatus();

  delete pageScheduler;
}

} // namespace search_engine
