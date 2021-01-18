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
  std::string url(spider->lastUrl());
  try {
    std::cout << "Store url: " + url << std::endl;
    PageStorage::storePage(directory, *spider, pageId);
    *success = true;
  } catch (std::exception &e) {
    std::cout << "Error while storing page " + url << std::endl;
    std::cout << e.what() << std::endl;
    *success = false;
  }
  std::cout << "Store finished: " + url << std::endl;
  counterFlag->signal();
}

} // namespace search_engine
