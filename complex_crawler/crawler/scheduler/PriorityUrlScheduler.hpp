#pragma once

#include <queue>
#include <string>
#include <utility>

namespace search_engine {

class UrlCompare {
public:
  UrlCompare();
  bool operator()(const std::pair<uint, std::string> &x,
                  const std::pair<uint, std::string> &y) const;
};

class PriorityUrlScheduler {
private:
  std::priority_queue<std::pair<uint, std::string>,
                      std::vector<std::pair<uint, std::string>>, UrlCompare>
      *priorityQueue;

  uint urlSizeCounter(std::string url);

public:
  PriorityUrlScheduler();
  ~PriorityUrlScheduler();
  std::string pop();
  void push(std::string url);
  int size();
};

} // namespace search_engine
