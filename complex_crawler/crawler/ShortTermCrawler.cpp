#include "ShortTermCrawler.hpp"
#include "../../threadpool/ThreadPool.hpp"
#include "../../utils/SynchronizedQueue.hpp"
#include "../../utils/Url.hpp"
#include "action/Crawl.hpp"
#include "scheduler/PageGroupScheduler.hpp"
#include "task/CrawlTask.hpp"
#include <CkSpider.h>
#include <chrono>
#include <iostream>
#include <map>
#include <string>

namespace search_engine {

ShortTermCrawler::ShortTermCrawler(std::string storageDirectory, bool verbose,
                                   int numThreads)
    : Crawler(storageDirectory, verbose), numThreads(numThreads) {
  this->pageScheduler = new PageGroupScheduler();
}

ShortTermCrawler::~ShortTermCrawler() { delete this->pageScheduler; }

void ShortTermCrawler::crawl(std::vector<std::string> &seedUrls,
                             std::size_t numPagesToCrawl) {
  this->pushUrlsIntoScheduler(seedUrls, numPagesToCrawl);

  auto pageGroupScheduler =
      dynamic_cast<PageGroupScheduler *>(this->pageScheduler);
  std::string url;
  auto *totalTimeMap = new std::map<std::string, double>();
  auto *lastCrawlEndTimeMap = new std::map<std::string, Crawl::timePoint>();
  // auto *spiderQueue = new utils::SynchronizedQueue<CkSpider>();
  // auto *crawlPool = new ThreadPool(this->numThreads);

  for (std::size_t i = 0; i < numPagesToCrawl; i++) {
    url = pageGroupScheduler->pop();

    bool useLastCrawlEndTime = true;

    std::string baseUrl = utils::baseUrl(url);
    auto it = lastCrawlEndTimeMap->find(baseUrl);
    if (it == lastCrawlEndTimeMap->end()) {
      totalTimeMap->operator[](baseUrl) = 0;
      lastCrawlEndTimeMap->operator[](baseUrl) =
          std::chrono::steady_clock::now();
      useLastCrawlEndTime = false;
    }

    CkSpider spider;
    double totalTime = totalTimeMap->operator[](baseUrl);
    Crawl::timePoint *lastCrawlEndTime =
        &(lastCrawlEndTimeMap->operator[](baseUrl));
    Crawl::crawlUrl(spider, url, this->mustMatchPatterns, this->avoidPatterns,
                    totalTime, *lastCrawlEndTime, useLastCrawlEndTime);

    pageGroupScheduler->finishWork(url);

    if (verbose) {
      std::cout << spider.lastUrl() << std::endl;
      std::cout << spider.get_NumUnspidered() << std::endl;
    }

    this->pushUrlsIntoScheduler(spider, numPagesToCrawl);
  }

  delete totalTimeMap;
  delete lastCrawlEndTimeMap;
  // delete spiderQueue;
  // delete crawlPool;
}

} // namespace search_engine
