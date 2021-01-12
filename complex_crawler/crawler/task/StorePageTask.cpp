#include "StorePageTask.hpp"
#include "../../../threadpool/CounterFlag.hpp"
#include "../action/PageStorage.hpp"
#include <iostream>
#include <string>

namespace search_engine {

StorePageTask::StorePageTask(CounterFlag *counterFlag, std::string directory,
                             std::string url, std::string html,
                             std::size_t pageId)
    : counterFlag(counterFlag), directory(directory), url(url), html(html),
      pageId(pageId) {}

StorePageTask::~StorePageTask() {}

void StorePageTask::run() {
  PageStorage::storePage(directory, url, html, pageId);
  counterFlag->signal();
  std::cout << "StorePageTask signal" << std::endl;
}

} // namespace search_engine
