#include "CrawlTask.hpp"
#include "../action/Crawl.hpp"
namespace search_engine {

CrawlTask::CrawlTask(utils::SynchronizedQueue<CkSpider> *queue,
                     CkSpider *spider, std::string *url,
                     std::vector<std::string> *mustMatchPatterns,
                     std::vector<std::string> *avoidPatterns, double *totalTime,
                     Crawl::timePoint *lastCrawlEndTime,
                     bool useLastCrawlEndTime)
    : queue(queue), spider(spider), url(url),
      mustMatchPatterns(mustMatchPatterns), avoidPatterns(avoidPatterns),
      totalTime(totalTime), lastCrawlEndTime(lastCrawlEndTime),
      useLastCrawlEndTime(useLastCrawlEndTime) {}

CrawlTask::~CrawlTask() {}

void CrawlTask::run() {
  Crawl::crawlUrl(*spider, *url, *mustMatchPatterns, *avoidPatterns, *totalTime,
                  *lastCrawlEndTime, useLastCrawlEndTime);
}

} // namespace search_engine
