#include "ShortTermCrawler.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "action/Crawl.hpp"
#include "scheduler/PageGroupScheduler.hpp"
#include <CkSpider.h>
#include <iostream>
#include <map>
#include <string>

namespace search_engine {

ShortTermCrawler::ShortTermCrawler(std::string storageDirectory, bool verbose)
    : Crawler(storageDirectory, verbose) {
  this->pageScheduler = new PageGroupScheduler();
}

ShortTermCrawler::~ShortTermCrawler() { delete this->pageScheduler; }

void ShortTermCrawler::crawl(std::vector<std::string> &seedUrls,
                             std::size_t numPagesToCrawl) {
  this->pushUrlsIntoScheduler(seedUrls, numPagesToCrawl);

  std::string url;
  auto *lastCrawlTimeMap = new std::map<std::string, Crawl::timePoint>();
  auto *totalTimeMap = new std::map<std::string, double>();
  auto *spiderQueue = new utils::SynchronizedQueue<CkSpider>();


  delete lastCrawlTimeMap;
  delete totalTimeMap;
  delete spiderQueue;
}

} // namespace search_engine
