#pragma once

#include "../Page.hpp"
#include "../scheduler/PageScheduler.hpp"
#include <CkSpider.h>
#include <map>
#include <pthread.h>
#include <string>
#include <vector>

namespace search_engine {

class PushIntoScheduler {
private:
  PushIntoScheduler() {}

public:
  static void push(PageScheduler *pageScheduler, Page &page,
                   std::map<std::string, bool> *viewedUrls,
                   pthread_mutex_t *memoryMutex = NULL);

  static void push(PageScheduler *pageScheduler, std::vector<std::string> &urls,
                   std::map<std::string, bool> *viewedUrls,
                   std::size_t numPagesToCrawl,
                   pthread_mutex_t *memoryMutex = NULL);

  static void push(PageScheduler *pageScheduler, CkSpider &spider,
                   std::map<std::string, bool> *viewedUrls,
                   std::size_t numPagesToCrawl, int pageLevel,
                   pthread_mutex_t *memoryMutex = NULL);
};

} // namespace search_engine
