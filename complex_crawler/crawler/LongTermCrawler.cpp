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

  PushIntoScheduler::push(pageScheduler, seedUrls, this->viewedUrls,
                          numPagesToCrawl);

  std::size_t numCrawledPages = 0;
  while (numCrawledPages < numPagesToCrawl) {
    std::string url, baseUrl;
    bool useLastCrawlEndTime;

    PopFromScheduler::pop(*pageScheduler, url, baseUrl, useLastCrawlEndTime,
                          *siteAttributesMap, *lastCrawlEndTimeMap);

    CkSpider spider;
    SiteAttributes *siteAttribute = &(siteAttributesMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    try {
      Crawl::crawlUrl(spider, url, this->mustMatchPatterns, this->avoidPatterns,
                      *siteAttribute, *lastCrawlEndTime, useLastCrawlEndTime);

      if (!useLastCrawlEndTime) { // first time crawling web site
        siteAttribute->addNumPagesLeve1(spider.get_NumUnspidered());
      }

      PageStorage::storePage(this->storageDirectory, spider, numCrawledPages);
      numCrawledPages++;
      PushIntoScheduler::push(pageScheduler, spider, this->viewedUrls,
                              numPagesToCrawl);
    } catch (std::exception &e) {
      std::cout << "Error when crawling page " + url << std::endl;
      std::cout << e.what() << std::endl;
    }
  }

  this->printCrawlStatus();

  delete pageScheduler;
}

} // namespace search_engine
