#include "Crawl.hpp"
#include "../../../utils/Assert.hpp"
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
  if (sleepTimeMs > 0) {
    spider.SleepMs(sleepTimeMs);
  }
}

void Crawl::crawlUrl(CkSpider &spider, std::string &url,
                     std::vector<std::string> &mustMatchPatterns,
                     std::vector<std::string> &avoidPatterns, double &totalTime,
                     Crawl::timePoint &lastCrawlEndTime,
                     bool useLastCrawlEndTime, pthread_mutex_t *memoryMutex) {
  bool crawlSuccess;
  Crawl::timePoint currentTime;
  Crawl::millis duration;

  std::string urlCopy("");
  urlCopy.append(url);

  search_engine::utils::assertTrue(spider.get_NumUnspidered() == 0,
                                   "Error(Crawl): invalid crawler");

  // std::cout << "Crawl initialize with " + urlCopy << std::endl;

  if (memoryMutex != NULL) {
    pthread_mutex_lock(memoryMutex);
    // std::cout << "Crawl lock memory " + urlCopy << std::endl
  }

  spider.Initialize(urlCopy.c_str());

  // std::cout << "Crawl initialize finish " + urlCopy << std::endl;

  for (auto mustMatchPattern : mustMatchPatterns) {
    spider.AddMustMatchPattern(mustMatchPattern.c_str());
  }

  for (auto avoidPattern : avoidPatterns) {
    spider.AddAvoidPattern(avoidPattern.c_str());
    spider.AddAvoidOutboundLinkPattern(avoidPattern.c_str());
  }

  search_engine::utils::assertTrue(
      spider.get_NumUnspidered() > 0,
      "Error(Crawl): invalid url get_NumUnspidered : " + urlCopy);

  spider.put_Utf8(true);

  if (memoryMutex != NULL) {
    // std::cout << "Crawl unlock memory " + urlCopy << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  // std::cout << "Crawl init finish " + urlCopy << std::endl;

  if (useLastCrawlEndTime) {
    Crawl::crawlSleepUntilMs(spider, lastCrawlEndTime);
  }

  if (memoryMutex != NULL) {
    pthread_mutex_lock(memoryMutex);
    // std::cout << "Crawl lock memory " + urlCopy << std::endl;
  }

  // std::cout << "Crawl craw begin " + urlCopy << std::endl;

  currentTime = std::chrono::steady_clock::now();
  crawlSuccess = spider.CrawlNext();
  lastCrawlEndTime = std::chrono::steady_clock::now();

  // std::cout << "Crawl craw finish " + urlCopy << std::endl;

  if (memoryMutex != NULL) {
    // std::cout << "Crawl unlock memory " + urlCopy << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  duration =
      std::chrono::duration_cast<Crawl::millis>(lastCrawlEndTime - currentTime);

  if (totalTime == 0) {
    totalTime += duration.count();
  }
  totalTime = (totalTime + duration.count()) / 2;

  search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());
}

void Crawl::crawlUrl(CkSpider &spider, std::string &url,
                            std::vector<std::string> &mustMatchPatterns,
                            std::vector<std::string> &avoidPatterns,
                            SiteAttributes &siteAttributes,
                            Crawl::timePoint &lastCrawlEndTime,
                            bool useLastCrawlEndTime,
                            pthread_mutex_t *memoryMutex) {
  bool crawlSuccess;
  Crawl::timePoint currentTime;
  Crawl::millis duration;

  std::string urlCopy("");
  urlCopy.append(url);

  search_engine::utils::assertTrue(spider.get_NumUnspidered() == 0,
                                   "Error(Crawl): invalid crawler");

  // std::cout << "Crawl initialize with " + urlCopy << std::endl;

  if (memoryMutex != NULL) {
    pthread_mutex_lock(memoryMutex);
    // std::cout << "Crawl lock memory " + urlCopy << std::endl
  }

  spider.Initialize(urlCopy.c_str());

  // std::cout << "Crawl initialize finish " + urlCopy << std::endl;

  for (auto mustMatchPattern : mustMatchPatterns) {
    spider.AddMustMatchPattern(mustMatchPattern.c_str());
  }

  for (auto avoidPattern : avoidPatterns) {
    spider.AddAvoidPattern(avoidPattern.c_str());
    spider.AddAvoidOutboundLinkPattern(avoidPattern.c_str());
  }

  search_engine::utils::assertTrue(
      spider.get_NumUnspidered() > 0,
      "Error(Crawl): invalid url get_NumUnspidered : " + urlCopy);

  spider.put_Utf8(true);

  if (memoryMutex != NULL) {
    // std::cout << "Crawl unlock memory " + urlCopy << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  // std::cout << "Crawl init finish " + urlCopy << std::endl;

  if (useLastCrawlEndTime) {
    Crawl::crawlSleepUntilMs(spider, lastCrawlEndTime);
  }

  if (memoryMutex != NULL) {
    pthread_mutex_lock(memoryMutex);
    // std::cout << "Crawl lock memory " + urlCopy << std::endl;
  }

  // std::cout << "Crawl craw begin " + urlCopy << std::endl;

  currentTime = std::chrono::steady_clock::now();
  crawlSuccess = spider.CrawlNext();
  lastCrawlEndTime = std::chrono::steady_clock::now();

  // std::cout << "Crawl craw finish " + urlCopy << std::endl;

  if (memoryMutex != NULL) {
    // std::cout << "Crawl unlock memory " + urlCopy << std::endl;
    pthread_mutex_unlock(memoryMutex);
  }

  duration =
      std::chrono::duration_cast<Crawl::millis>(lastCrawlEndTime - currentTime);

  siteAttributes.addToTotalTime(duration.count());
  siteAttributes.addNumPages(1);
  std::string html(spider.lastHtml());
  siteAttributes.addToTotalPageSize(html.size());

  search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());
}

} // namespace search_engine
