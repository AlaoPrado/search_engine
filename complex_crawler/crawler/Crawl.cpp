#include "Crawl.hpp"
#include "../../utils/Assert.hpp"
#include <CkSpider.h>
#include <chrono>
#include <string>
#include <vector>

namespace search_engine {

void Crawl::crawlSleepUntilMs(CkSpider &spider,
                              Crawl::timePoint &lastCrawlEndTime,
                              Crawl::timePoint &currentTime) {
  Crawl::millis duration =
      std::chrono::duration_cast<Crawl::millis>(currentTime - lastCrawlEndTime);
  int sleepTimeMs = Crawl::CRAWL_AWAIT_TIME_MS - duration.count();
  if (sleepTimeMs > 0) {
    spider.SleepMs(sleepTimeMs);
  }
}

void Crawl::crawlUrl(CkSpider &spider, std::string &url,
                     std::vector<std::string> &mustMatchPatterns,
                     std::vector<std::string> &avoidPatterns, double &totalTime,
                     Crawl::timePoint &lastCrawlEndTime,
                     bool useLastCrawlEndTime) {

  bool crawlSuccess;
  Crawl::timePoint currentTime;
  Crawl::millis duration;

  spider.Initialize(url.c_str());

  for (auto mustMatchPattern : mustMatchPatterns) {
    spider.AddMustMatchPattern(mustMatchPattern.c_str());
  }

  for (auto avoidPattern : avoidPatterns) {
    spider.AddAvoidPattern(avoidPattern.c_str());
    spider.AddAvoidOutboundLinkPattern(avoidPattern.c_str());
  }

  search_engine::utils::assertTrue(spider.get_NumUnspidered() > 0,
                                   "Error: invalid url");

  spider.put_Utf8(true);

  if (useLastCrawlEndTime) {
    currentTime = std::chrono::steady_clock::now();
    Crawl::crawlSleepUntilMs(spider, lastCrawlEndTime, currentTime);
  }

  currentTime = std::chrono::steady_clock::now();
  crawlSuccess = spider.CrawlNext();
  lastCrawlEndTime = std::chrono::steady_clock::now();

  duration =
      std::chrono::duration_cast<Crawl::millis>(lastCrawlEndTime - currentTime);
  totalTime += duration.count();

  search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());
}

} // namespace search_engine
