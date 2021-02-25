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
  std::cout << "CrawlTask " + page.getUrl() << std::endl;
  bool success;
  try {
    Crawl::crawlUrl(*spider, page, *mustMatchPatterns, *avoidPatterns,
                    *siteAttribute, *lastCrawlEndTime, useLastCrawlEndTime);
    std::cout << "CrawlTask success: " + page.getUrl() << std::endl;
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
  std::cout << "CrawlTask finish: " + page.getUrl() << std::endl;
}

} // namespace search_engine
