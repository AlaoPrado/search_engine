#pragma once

#include "../../../threadpool/CounterFlag.hpp"
#include "../../../threadpool/Task.hpp"
#include <string>
namespace search_engine {

class StorePageTask : public Task {
private:
  CounterFlag *counterFlag;
  std::string directory;
  std::string url, html;
  std::size_t pageId;

public:
  StorePageTask(CounterFlag *counterFlag, std::string directory,
                std::string url, std::string html, std::size_t pageId);
  ~StorePageTask();
  void run() override;
};

} // namespace search_engine
