#pragma once

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

class PriorityUrlScheduler {
private:
  std::priority_queue<UrlEntry, std::vector<UrlEntry>, UrlCompare>
      *priorityQueue;

  std::size_t countUrlSize(std::string url);

public:
  PriorityUrlScheduler();
  ~PriorityUrlScheduler();
  std::string pop();
  void push(std::string url);
  int size();
};

} // namespace search_engine
