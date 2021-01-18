#include "CrawlTask.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "../action/Crawl.hpp"
#include "CrawlTaskResult.hpp"
#include <exception>
#include <iostream>
#include <pthread.h>
namespace search_engine {

CrawlTask::CrawlTask(pthread_mutex_t *memoryMutex,
                     utils::SynchronizedQueue<CrawlTaskResult> *queue,
                     CkSpider *spider, Page page,
                     std::vector<std::string> *mustMatchPatterns,
                     std::vector<std::string> *avoidPatterns,
                     SiteAttributes *siteAttribute,
                     Crawl::timePoint *lastCrawlEndTime,
                     bool useLastCrawlEndTime)
    : memoryMutex(memoryMutex), queue(queue), spider(spider), page(page),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      siteAttribute(siteAttribute), lastCrawlEndTime(lastCrawlEndTime),
      useLastCrawlEndTime(useLastCrawlEndTime) {}

CrawlTask::~CrawlTask() {}

void CrawlTask::run() {
  // std::cout << "CrawlTask " + url << std::endl;
  bool success;
  try {
    Crawl::crawlUrl(*spider, page, *mustMatchPatterns, *avoidPatterns,
                    *siteAttribute, *lastCrawlEndTime, useLastCrawlEndTime,
                    memoryMutex);
    success = true;
  } catch (std::exception &e) {
    std::cout << "Error when crawling page " + page.getUrl() << std::endl;
    std::cout << e.what() << std::endl;
    success = false;
  }

  // std::cout << "CrawlTask push spider " + url  << std::endl;
  pthread_mutex_lock(memoryMutex);
  CrawlTaskResult *crawlTaskResult =
      new CrawlTaskResult(spider, page, siteAttribute, success);
  pthread_mutex_unlock(memoryMutex);

  queue->push(crawlTaskResult);
}

} // namespace search_engine
