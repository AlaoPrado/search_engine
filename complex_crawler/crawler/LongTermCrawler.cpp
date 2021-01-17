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
      Crawl::crawlUrl(spider, page, this->mustMatchPatterns, this->avoidPatterns,
                      *siteAttributes, *lastCrawlEndTime, useLastCrawlEndTime);

      PageStorage::storePage(this->storageDirectory, spider, numCrawledPages);
      numCrawledPages++;
      PushIntoScheduler::push(pageScheduler, spider, this->viewedUrls,
                              numPagesToCrawl);
    } catch (std::exception &e) {
      std::cout << "Error when crawling page " + page.getUrl() << std::endl;
      std::cout << e.what() << std::endl;
    }
  }

  this->printCrawlStatus();

  delete pageScheduler;
}

} // namespace search_engine
