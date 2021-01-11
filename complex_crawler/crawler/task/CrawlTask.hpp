#pragma once

#include "../../../threadpool/Task.hpp"
#include "../action/Crawl.hpp"
#include <CkSpider>
#include <string>
#include <vector>

namespace search_engine {

class CrawlTask : public Task {
private:
  CkSpider *spider;
  std::string *url;
  std::vector<std::string> *mustMatchPatterns;
  std::vector<std::string> *avoidPatterns;
  double *totalTime;
  Crawl::timePoint *lastCrawlEndTime;
  bool useLastCrawlEndTime;

public:
  CrawlTask(CkSpider *spider, std::string *url,
            std::vector<std::string> *mustMatchPatterns,
            std::vector<std::string> *avoidPatterns, double *totalTime,
            Crawl::timePoint *lastCrawlEndTime, bool useLastCrawlEndTime);
  ~CrawlTask();
  void run() override;
};

} // namespace search_engine
