#include "LongTermCrawler.hpp"
#include "../../utils/Url.hpp"
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
  Crawl::timePoint lastCrawlTime;
  double totalTime = 0;

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    url = this->pageScheduler->pop();
    CkSpider spider;
    Crawl::crawlUrl(spider, url, this->mustMatchPatterns, this->avoidPatterns,
                    totalTime, lastCrawlTime, totalTime > 0);
    if (verbose) {
      std::cout << spider.lastUrl() << std::endl;
      std::cout << spider.get_NumUnspidered() << std::endl;
    }

    PageStorage::storePage(this->storageDirectory, spider, i);
    PushIntoScheduler::push(this->pageScheduler, spider, this->viewedUrls,
                            numPagesToCrawl);
  }

  delete this->pageScheduler;
}

} // namespace search_engine
