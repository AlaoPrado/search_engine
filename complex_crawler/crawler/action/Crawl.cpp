#include "Crawl.hpp"
#include "../../../utils/Assert.hpp"
#include "../Page.hpp"
#include <CkSpider.h>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

namespace search_engine {

void Crawl::crawlSleepUntilMs(CkSpider &spider,
                              Crawl::timePoint &lastCrawlEndTime) {
  Crawl::timePoint currentTime = std::chrono::steady_clock::now();
  Crawl::millis duration =
      std::chrono::duration_cast<Crawl::millis>(currentTime - lastCrawlEndTime);
  int sleepTimeMs = Crawl::CRAWL_AWAIT_TIME_MS - duration.count();
  // std::cout << "SleepTimems" + std::to_string(sleepTimeMs) << std::endl;
  if (sleepTimeMs > 0) {
    spider.SleepMs(sleepTimeMs);
  }
}

void Crawl::crawlUrl(CkSpider &spider, Page &page,
                     std::vector<std::string> &mustMatchPatterns,
                     std::vector<std::string> &avoidPatterns,
                     SiteAttributes &siteAttributes,
                     Crawl::timePoint &lastCrawlEndTime,
                     bool useLastCrawlEndTime, pthread_mutex_t *memoryMutex) {
  bool crawlSuccess;
  Crawl::timePoint currentTime;
  Crawl::millis duration;

  std::string url("");
  url.append(page.getUrl());

  search_engine::utils::assertTrue(spider.get_NumUnspidered() == 0,
                                   "Error(Crawl): invalid crawler");

  // std::cout << "Crawl initialize with " + url << std::endl;

  if (memoryMutex != NULL) {
    pthread_mutex_lock(memoryMutex);
    // std::cout << "Crawl lock memory " + url << std::endl
  }

  spider.Initialize(url.c_str());

  // std::cout << "Crawl initialize finish " + url << std::endl;

  for (auto mustMatchPattern : mustMatchPatterns) {
    spider.AddMustMatchPattern(mustMatchPattern.c_str());
  }

  for (auto avoidPattern : avoidPatterns) {
    spider.AddAvoidPattern(avoidPattern.c_str());
    spider.AddAvoidOutboundLinkPattern(avoidPattern.c_str());
  }

  search_engine::utils::assertTrue(
      spider.get_NumUnspidered() > 0,
      "Error(Crawl): invalid url get_NumUnspidered : " + url);

  spider.put_Utf8(true);

  if (memoryMutex != NULL) {
    // std::cout << "Crawl unlock memory " + url << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  // std::cout << "Crawl init finish " + url << std::endl;

  if (useLastCrawlEndTime) {
    Crawl::crawlSleepUntilMs(spider, lastCrawlEndTime);
  }

  currentTime = std::chrono::steady_clock::now();

  if (memoryMutex != NULL) {
    pthread_mutex_lock(memoryMutex);
    // std::cout << "Crawl lock memory " + url << std::endl;
  }

  crawlSuccess = spider.CrawlNext();

  if (memoryMutex != NULL) {
    // std::cout << "Crawl unlock memory " + url << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  lastCrawlEndTime = std::chrono::steady_clock::now();

  search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());

  duration =
      std::chrono::duration_cast<Crawl::millis>(lastCrawlEndTime - currentTime);

  siteAttributes.addToTotalTime(duration.count());
  siteAttributes.addNumPages(1);
  std::string html(spider.lastHtml());
  siteAttributes.addToTotalPageSize(html.size());

  if (page.getLevel() == 0) {
    siteAttributes.addNumPagesLevel1(spider.get_NumUnspidered() +
                                     spider.get_NumOutboundLinks());
  }
}

} // namespace search_engine
