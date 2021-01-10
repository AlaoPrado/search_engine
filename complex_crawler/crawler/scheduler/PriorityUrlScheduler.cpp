#include "PriorityUrlScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include <exception>

namespace search_engine {

UrlCompare::UrlCompare() {}

bool UrlCompare::operator()(const std::pair<uint, std::string> &x,
                            const std::pair<uint, std::string> &y) const {
  return x.first < y.first;
}

PriorityUrlScheduler::PriorityUrlScheduler() {
  this->priorityQueue =
      new std::priority_queue<std::pair<uint, std::string>,
                              std::vector<std::pair<uint, std::string>>,
                              UrlCompare>();
}

PriorityUrlScheduler::~PriorityUrlScheduler() { delete this->priorityQueue; }

uint PriorityUrlScheduler::urlSizeCounter(std::string url) {
  const std::string protocol = "http://";
  std::size_t position = url.find(protocol);

  utils::assertTrue(position == 0, "Error: invalid url");

  const std::string separator = "/";
  uint urltSize = 0;
  position = url.find(separator, protocol.length());
  while (position != std::string::npos) {
    urltSize++;
    position = url.find(separator, position + separator.length());
  }
  return urltSize;
}

void PriorityUrlScheduler::push(std::string url) {
  try {
    uint urlSize = this->urlSizeCounter(url);
    this->priorityQueue->push(std::make_pair(urlSize, url));
  } catch (std::exception &e) {
    // ignore exception
  }
}

std::string PriorityUrlScheduler::pop() {
  std::pair<uint, std::string> urlPair = this->priorityQueue->top();
  this->priorityQueue->pop();
  return urlPair.second;
}

int PriorityUrlScheduler::size() { return this->priorityQueue->size(); }

} // namespace search_engine
