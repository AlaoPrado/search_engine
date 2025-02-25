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
#include "PopFromScheduler.hpp"
#include "../../../utils/Url.hpp"
#include "../Page.hpp"
#include "../SiteAttributes.hpp"
#include "Crawl.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <pthread.h>
#include <string>

namespace search_engine {

void PopFromScheduler::pop(
    PageScheduler &pageScheduler, Page &page, std::string &baseUrl,
    bool &useLastCrawlEndTime,
    std::map<std::string, SiteAttributes> &siteAttributesMap,
    std::map<std::string, Crawl::timePoint> &lastCrawlEndTimeMap,
    pthread_mutex_t *memoryMutex, pthread_mutex_t *popMutex) {
  page = pageScheduler.pop();
  baseUrl = utils::baseUrl(page.getUrl());

  if (popMutex != NULL) {
    pthread_mutex_lock(popMutex);
  }

  auto it = lastCrawlEndTimeMap.find(baseUrl);
  if (it == lastCrawlEndTimeMap.end()) {
    if (memoryMutex != NULL) {
      pthread_mutex_lock(memoryMutex);
    }

    siteAttributesMap.operator[](baseUrl) = SiteAttributes();
    lastCrawlEndTimeMap.operator[](baseUrl) = std::chrono::steady_clock::now();

    if (memoryMutex != NULL) {
      pthread_mutex_unlock(memoryMutex);
    }

    useLastCrawlEndTime = false;
  } else {
    useLastCrawlEndTime = true;
  }

  if (popMutex != NULL) {
    pthread_mutex_unlock(popMutex);
  }
}

} // namespace search_engine
