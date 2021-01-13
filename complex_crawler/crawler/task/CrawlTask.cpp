#include "CrawlTask.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include <iostream>
#include <pthread.h>
namespace search_engine {

CrawlTask::CrawlTask(pthread_mutex_t *memoryMutex,
                     utils::SynchronizedQueue<CkSpider> *queue,
                     CkSpider *spider, std::string url,
                     std::vector<std::string> *mustMatchPatterns,
                     std::vector<std::string> *avoidPatterns,
                     SiteAttributes *siteAttribute,
                     Crawl::timePoint *lastCrawlEndTime,
                     bool useLastCrawlEndTime, pthread_mutex_t *crawlMutex)
    : memoryMutex(memoryMutex), queue(queue), spider(spider), url(url),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      siteAttribute(siteAttribute), lastCrawlEndTime(lastCrawlEndTime),
      useLastCrawlEndTime(useLastCrawlEndTime), crawlMutex(crawlMutex) {}

CrawlTask::~CrawlTask() {}

void CrawlTask::run() {
  // std::cout << "CrawlTask " + url << std::endl;
  try {
    Crawl::crawlUrl(*spider, url, *mustMatchPatterns, *avoidPatterns,
                    *siteAttribute, *lastCrawlEndTime, useLastCrawlEndTime,
                    memoryMutex);
  } catch (std::exception &e) {
    std::cout << "Error when crawling page " + url << std::endl;
    std::cout << e.what() << std::endl;
  }

  // std::cout << "CrawlTask push spider " + url  << std::endl;
  queue->push(spider);
}

} // namespace search_engine
