#pragma once

#include "PageScheduler.hpp"
#include "UrlEntry.hpp"
#include <queue>
#include <string>
#include <vector>

namespace search_engine {

class UrlCompare {
public:
  UrlCompare();
  bool operator()(UrlEntry &x, UrlEntry &y) const;
};

class PriorityPageScheduler : public PageScheduler {
private:
  std::priority_queue<UrlEntry, std::vector<UrlEntry>, UrlCompare>
      *priorityQueue;

public:
  PriorityPageScheduler();
  virtual ~PriorityPageScheduler() override;
  virtual std::string pop() override;
  virtual void push(std::string url) override;
  virtual std::size_t size() override;
  virtual bool empty() override;
};

} // namespace search_engine
