#include "PriorityUrlScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
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

void PriorityUrlScheduler::push(std::string url) {
  std::size_t urlSize = utils::countUrlSize(url);
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
