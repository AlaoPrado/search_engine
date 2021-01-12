#include "CrawlTask.hpp"
#include "../action/Crawl.hpp"
#include <iostream>
#include <pthread.h>
namespace search_engine {

CrawlTask::CrawlTask(pthread_mutex_t *memoryMutex,
                     utils::SynchronizedQueue<CkSpider> *queue,
                     CkSpider *spider, std::string url,
                     std::vector<std::string> *mustMatchPatterns,
                     std::vector<std::string> *avoidPatterns, double *totalTime,
                     Crawl::timePoint *lastCrawlEndTime,
                     bool useLastCrawlEndTime, pthread_mutex_t *crawlMutex)
    : memoryMutex(memoryMutex), queue(queue), spider(spider), url(url),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      totalTime(totalTime), lastCrawlEndTime(lastCrawlEndTime),
      useLastCrawlEndTime(useLastCrawlEndTime), crawlMutex(crawlMutex) {}

CrawlTask::~CrawlTask() {}

void CrawlTask::run() {
  // std::cout << "CrawlTask " + url << std::endl;
  // pthread_mutex_lock(crawlMutex);
  Crawl::crawlUrl(*spider, url, *mustMatchPatterns, *avoidPatterns, *totalTime,
                  *lastCrawlEndTime, useLastCrawlEndTime, memoryMutex);
  // pthread_mutex_unlock(crawlMutex);
  // std::cout << "CrawlTask push spider " + url  << std::endl;
  queue->push(spider);
}

} // namespace search_engine
