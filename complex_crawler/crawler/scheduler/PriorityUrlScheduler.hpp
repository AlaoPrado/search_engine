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

class PriorityUrlScheduler : public PageScheduler {
private:
  std::priority_queue<UrlEntry, std::vector<UrlEntry>, UrlCompare>
      *priorityQueue;

public:
  PriorityUrlScheduler();
  ~PriorityUrlScheduler();
  virtual std::string pop() override;
  virtual void push(std::string url) override;
  int size();
};

} // namespace search_engine
