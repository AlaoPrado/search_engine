#pragma once

#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace search_engine {

class UrlCompare {
public:
  UrlCompare();
  bool operator()(const std::pair<std::size_t, std::string> &x,
                  const std::pair<std::size_t, std::string> &y) const;
};

class PriorityUrlScheduler {
private:
  std::priority_queue<std::pair<std::size_t, std::string>,
                      std::vector<std::pair<std::size_t, std::string>>,
                      UrlCompare> *priorityQueue;

  std::size_t countUrlSize(std::string url);

public:
  PriorityUrlScheduler();
  ~PriorityUrlScheduler();
  std::string pop();
  void push(std::string url);
  int size();
};

} // namespace search_engine
