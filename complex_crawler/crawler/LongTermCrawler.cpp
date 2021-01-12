#include "LongTermCrawler.hpp"
#include "../../utils/Url.hpp"
#include "SiteAttributes.hpp"
#include "action/Crawl.hpp"
#include "action/PageStorage.hpp"
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
  this->pageScheduler = new PriorityPageScheduler();

  PushIntoScheduler::push(this->pageScheduler, seedUrls, this->viewedUrls,
                          numPagesToCrawl);

  std::string url;
  auto *siteAttributesMap = new std::map<std::string, SiteAttributes>();
  auto *lastCrawlEndTimeMap = new std::map<std::string, Crawl::timePoint>();

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    url = this->pageScheduler->pop();

    bool useLastCrawlEndTime = true;
    std::string baseUrl = utils::baseUrl(url);

    auto it = lastCrawlEndTimeMap->find(baseUrl);
    if (it == lastCrawlEndTimeMap->end()) {
      siteAttributesMap->operator[](baseUrl) = SiteAttributes();
      lastCrawlEndTimeMap->operator[](baseUrl) =
          std::chrono::steady_clock::now();
      useLastCrawlEndTime = false;
    }

    SiteAttributes *siteAttribute = &(siteAttributesMap->operator[](baseUrl));
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));

    CkSpider spider;
    Crawl::crawlUrl(spider, url, this->mustMatchPatterns, this->avoidPatterns,
                    *siteAttribute, *lastCrawlEndTime, useLastCrawlEndTime);

    if (!useLastCrawlEndTime) { // firt time crawling web site
      siteAttribute->addNumPagesLeve1(spider.get_NumUnspidered());
    }

    PageStorage::storePage(this->storageDirectory, spider, i);
    PushIntoScheduler::push(this->pageScheduler, spider, this->viewedUrls,
                            numPagesToCrawl);
  }

  if (verbose) {
    for (auto it = siteAttributesMap->begin(); it != siteAttributesMap->end();
         it++) {
      std::cout << "Web site " << it->first << std::endl;
      std::cout << "Number of URLs at level 1 crawled: "
                << it->second.getNumPagesLeve1() << std::endl;
      std::cout << "Average crawl time for (milliseconds): "
                << it->second.getAverageTime() << std::endl;
      std::cout << "Average page size (Bytes): "
                << it->second.getAveragePageSize() << std::endl;
    }
  }

  delete this->pageScheduler;
  delete siteAttributesMap;
  delete lastCrawlEndTimeMap;
}

} // namespace search_engine
