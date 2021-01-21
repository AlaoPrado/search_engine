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

  static bool pushedSufficient(std::map<std::string, bool> &viewedUrls,
                               std::size_t numPagesToCrawl);

public:
  static void push(PageScheduler &pageScheduler, Page &page,
                   std::map<std::string, bool> &viewedUrls, int &numPagesPushed,
                   pthread_mutex_t *memoryMutex = NULL,
                   pthread_mutex_t *pushMutex = NULL);

  static void push(PageScheduler &pageScheduler, std::vector<std::string> &urls,
                   std::map<std::string, bool> &viewedUrls,
                   std::size_t numPagesToCrawl, int &numPagesPushed,
                   pthread_mutex_t *memoryMutex = NULL,
                   pthread_mutex_t *pushMutex = NULL);

  static void push(PageScheduler &pageScheduler, CkSpider &spider,
                   std::map<std::string, bool> &viewedUrls,
                   std::size_t numPagesToCrawl, int pageLevel,
                   int &numPagesPushed, pthread_mutex_t *memoryMutex = NULL,
                   pthread_mutex_t *pushMutex = NULL);
};

} // namespace search_engine
