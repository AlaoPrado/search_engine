#include "PriorityUrlScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include <string>
#include <vector>

namespace search_engine {

UrlCompare::UrlCompare() {}

bool UrlCompare::operator()(
    const std::pair<std::size_t, std::string> &x,
    const std::pair<std::size_t, std::string> &y) const {
  return x.first < y.first;
}

PriorityUrlScheduler::PriorityUrlScheduler() {
  this->priorityQueue =
      new std::priority_queue<std::pair<std::size_t, std::string>,
                              std::vector<std::pair<std::size_t, std::string>>,
                              UrlCompare>();
}

PriorityUrlScheduler::~PriorityUrlScheduler() { delete this->priorityQueue; }

std::size_t PriorityUrlScheduler::countUrlSize(std::string url) {
  const std::vector<std::string> validProtocols = {"http://", "https://"};

  std::size_t position = std::string::npos;
  std::string protocol;
  for (auto currentProtocol : validProtocols) {
    position = url.find(currentProtocol);
    if (position == 0) {
      protocol = currentProtocol;
      break;
    }
  }
  utils::assertTrue(position == 0, "Error: invalid url");

  const std::string separator = "/";
  std::size_t urltSize = 0;
  position = url.find(separator, protocol.length());
  while (position != std::string::npos) {
    urltSize++;
    position = url.find(separator, position + separator.length());
  }
  return urltSize;
}

void PriorityUrlScheduler::push(std::string url) {
  std::size_t urlSize = this->countUrlSize(url);
  this->priorityQueue->push(std::make_pair(urlSize, url));
}

std::string PriorityUrlScheduler::pop() {
  std::pair<std::size_t, std::string> urlPair = this->priorityQueue->top();
  this->priorityQueue->pop();
  return urlPair.second;
}

int PriorityUrlScheduler::size() { return this->priorityQueue->size(); }

} // namespace search_engine
