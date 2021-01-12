#pragma once

#include "../../../threadpool/Task.hpp"
#include "../../../utils/SynchronizedQueue.hpp"
#include "../action/Crawl.hpp"
#include <CkSpider.h>
#include <string>
#include <vector>

namespace search_engine {

class CrawlTask : public Task {
private:
  utils::SynchronizedQueue<CkSpider> *queue;
  CkSpider *spider;
  std::string *url;
  std::vector<std::string> *mustMatchPatterns;
  std::vector<std::string> *avoidPatterns;
  double *totalTime;
  Crawl::timePoint *lastCrawlEndTime;
  bool useLastCrawlEndTime;

public:
  CrawlTask(utils::SynchronizedQueue<CkSpider> *queue, CkSpider *spider,
            std::string *url, std::vector<std::string> *mustMatchPatterns,
            std::vector<std::string> *avoidPatterns, double *totalTime,
            Crawl::timePoint *lastCrawlEndTime, bool useLastCrawlEndTime);
  ~CrawlTask();
  void run() override;
  CkSpider *getSpider();
};

} // namespace search_engine
