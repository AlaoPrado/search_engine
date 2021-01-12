#include "StorePageTask.hpp"
#include "../../../threadpool/CounterFlag.hpp"
#include "../action/PageStorage.hpp"
#include <CkSpider.h>
#include <string>

namespace search_engine {

StorePageTask::StorePageTask(CounterFlag *counterFlag, std::string directory,
                             CkSpider *spider,
                             std::size_t pageId)
    : counterFlag(counterFlag), directory(directory), spider(spider),
      pageId(pageId) {}

StorePageTask::~StorePageTask() {}

void StorePageTask::run() {
  PageStorage::storePage(directory, *spider, pageId);
  counterFlag->signal();
}

} // namespace search_engine
