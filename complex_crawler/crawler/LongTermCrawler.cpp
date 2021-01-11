#include "LongTermCrawler.hpp"
#include "../../utils/Url.hpp"
#include "Crawl.hpp"
#include "scheduler/PriorityPageScheduler.hpp"
#include <CkSpider.h>
#include <chrono>
#include <exception>
#include <iostream>
namespace search_engine {

LongTermCrawler::LongTermCrawler(bool verbose) : Crawler(verbose) {
  this->pageScheduler = new PriorityPageScheduler();
}

LongTermCrawler::~LongTermCrawler() { delete this->pageScheduler; }

void LongTermCrawler::crawl(std::vector<std::string> &seedUrls,
                            std::size_t numPagesToCrawl) {

  this->pushUrlsIntoScheduler(seedUrls, numPagesToCrawl);

  std::string url;
  Crawl::timePoint lastCrawlTime;
  double totalTime = 0;

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    url = this->pageScheduler->pop();
    CkSpider spider;
    Crawl::crawlUrl(spider, url, this->mustMatchPatterns, this->avoidPatterns,
                    totalTime, lastCrawlTime, totalTime > 0);
    std::cout << spider.lastUrl() << std::endl;
    std::cout << spider.lastHtmlTitle() << std::endl;
    std::cout << spider.get_NumUnspidered() << std::endl;
    this->pushUrlsIntoScheduler(spider, numPagesToCrawl);
  }
}

} // namespace search_engine
