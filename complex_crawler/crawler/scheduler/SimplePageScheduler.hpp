#pragma once

#include "../Page.hpp"
#include "PageEntry.hpp"
#include "PriorityPageScheduler.hpp"
#include <queue>
#include <string>

namespace search_engine {

class SimplePageScheduler : public PageScheduler {
private:
  std::queue<PageEntry> *urlQueue;

public:
  SimplePageScheduler();
  virtual ~SimplePageScheduler() override;
  virtual Page pop() override;
  virtual void push(Page page) override;
  virtual std::size_t size() override;
  virtual bool empty() override;
};

} // namespace search_engine
