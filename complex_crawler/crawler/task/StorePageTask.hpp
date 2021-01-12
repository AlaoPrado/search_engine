#pragma once

#include "../../../threadpool/Task.hpp"

namespace search_engine {

class StorePageTask : public Task {
private:
public:
  StorePageTask();
  ~StorePageTask();
  void run() override;
};

} // namespace search_engine
