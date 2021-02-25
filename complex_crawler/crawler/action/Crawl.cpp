/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
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
                     bool useLastCrawlEndTime) {
  bool crawlSuccess;
  Crawl::timePoint currentTime;
  Crawl::millis duration;

  std::string url("");
  url.append(page.getUrl());

  search_engine::utils::assertTrue(spider.get_NumUnspidered() == 0,
                                   "Error(Crawl): invalid crawler");

  // std::cout << "Crawl initialize with " + url << std::endl;

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

  // std::cout << "Crawl init finish " + url << std::endl;

  if (useLastCrawlEndTime) {
    Crawl::crawlSleepUntilMs(spider, lastCrawlEndTime);
  }

  currentTime = std::chrono::steady_clock::now();
  crawlSuccess = spider.CrawlNext();
  lastCrawlEndTime = std::chrono::steady_clock::now();

  search_engine::utils::assertTrue(crawlSuccess, spider.lastErrorText());

  duration =
      std::chrono::duration_cast<Crawl::millis>(lastCrawlEndTime - currentTime);

  siteAttributes.addToTotalTime(duration.count());
  siteAttributes.addNumPages(1);
  std::string html(spider.lastHtml());
  siteAttributes.addToTotalPageSize(html.size());
}

} // namespace search_engine
