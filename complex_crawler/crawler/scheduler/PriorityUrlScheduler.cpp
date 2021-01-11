#include "PriorityUrlScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include <string>
#include <vector>
namespace search_engine {

UrlCompare::UrlCompare() {}

bool UrlCompare::operator()(UrlEntry &x, UrlEntry &y) const {
  return x.getSize() > y.getSize();
}

PriorityUrlScheduler::PriorityUrlScheduler() {
  this->priorityQueue =
      new std::priority_queue<UrlEntry, std::vector<UrlEntry>, UrlCompare>();
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
  utils::assertTrue(position == 0,
                    "Error(PriorityUrlScheduler): invalid url protocol");

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
  this->priorityQueue->push(UrlEntry(url, urlSize));
}

std::string PriorityUrlScheduler::pop() {
  utils::assertTrue(this->size() > 0,
                    "Error(PriorityUrlScheduler): pop when scheduler is empty");
  UrlEntry urlEntry = this->priorityQueue->top();
  this->priorityQueue->pop();
  return urlEntry.getUrl();
}

int PriorityUrlScheduler::size() { return this->priorityQueue->size(); }

} // namespace search_engine
