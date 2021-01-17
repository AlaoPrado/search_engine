#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include <CkSpider.h>
#include <string>
namespace search_engine {

class StorePageTask : public Task {
private:
  CounterFlag *counterFlag;
  std::string directory;
  CkSpider *spider;
  std::size_t pageId;
  bool *success;

public:
  StorePageTask(CounterFlag *counterFlag, std::string directory,
                CkSpider *spider, std::size_t pageId, bool *success);
  ~StorePageTask();
  void run() override;
};

} // namespace search_engine
