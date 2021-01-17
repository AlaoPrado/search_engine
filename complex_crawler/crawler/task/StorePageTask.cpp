#include "StorePageTask.hpp"
#include "../../../threadpool/CounterFlag.hpp"
#include "../action/PageStorage.hpp"
#include <CkSpider.h>
#include <iostream>
#include <string>

namespace search_engine {

StorePageTask::StorePageTask(CounterFlag *counterFlag, std::string directory,
                             CkSpider *spider, std::size_t pageId,
                             bool *success)
    : counterFlag(counterFlag), directory(directory), spider(spider),
      pageId(pageId), success(success) {}

StorePageTask::~StorePageTask() {}

void StorePageTask::run() {
  try {
    PageStorage::storePage(directory, *spider, pageId);
    *success = true;
  } catch (std::exception &e) {
    std::string url(spider->lastUrl());
    std::cout << "Error when crawling page " + url << std::endl;
    std::cout << e.what() << std::endl;
    *success = false;
  }
  counterFlag->signal();
}

} // namespace search_engine
