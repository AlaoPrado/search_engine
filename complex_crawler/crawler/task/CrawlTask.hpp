#pragma once

#include "../../../threadpool/Task.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../action/Crawl.hpp"
#include <CkSpider.h>
#include <pthread.h>
#include <string>
#include <vector>

namespace search_engine {

class CrawlTask : public Task {
private:
  pthread_mutex_t *memoryMutex;
  utils::SynchronizedQueue<CkSpider> *queue;
  CkSpider *spider;
  std::string url;
  std::vector<std::string> *mustMatchPatterns;
  std::vector<std::string> *avoidPatterns;
  double *totalTime;
  Crawl::timePoint *lastCrawlEndTime;
  bool useLastCrawlEndTime;
  pthread_mutex_t *crawlMutex;

public:
  CrawlTask(pthread_mutex_t *memoryMutex,
            utils::SynchronizedQueue<CkSpider> *queue, CkSpider *spider,
            std::string url, std::vector<std::string> *mustMatchPatterns,
            std::vector<std::string> *avoidPatterns, double *totalTime,
            Crawl::timePoint *lastCrawlEndTime, bool useLastCrawlEndTime,
            pthread_mutex_t *crawlMutex);
  ~CrawlTask();
  void run() override;
};

} // namespace search_engine
