#pragma once

#include "../Page.hpp"
#include "PageEntry.hpp"
#include "PageScheduler.hpp"
#include <queue>
#include <string>
#include <vector>

namespace search_engine {

class PageCompare {
public:
  PageCompare();
  bool operator()(PageEntry &x, PageEntry &y) const;
};

class PriorityPageScheduler : public PageScheduler {
private:
  std::priority_queue<PageEntry, std::vector<PageEntry>, PageCompare>
      *priorityQueue;

public:
  PriorityPageScheduler();
  virtual ~PriorityPageScheduler() override;
  virtual Page pop() override;
  virtual void push(Page page) override;
  virtual std::size_t size() override;
  virtual bool empty() override;
};

} // namespace search_engine
