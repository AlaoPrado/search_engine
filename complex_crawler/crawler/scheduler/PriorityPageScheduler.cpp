#include "PriorityPageScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include <string>
#include <vector>
namespace search_engine {

UrlCompare::UrlCompare() {}

bool UrlCompare::operator()(UrlEntry &x, UrlEntry &y) const {
  return x.getSize() > y.getSize();
}

PriorityPageScheduler::PriorityPageScheduler() {
  this->priorityQueue =
      new std::priority_queue<UrlEntry, std::vector<UrlEntry>, UrlCompare>();
}

PriorityPageScheduler::~PriorityPageScheduler() { delete this->priorityQueue; }

void PriorityPageScheduler::push(std::string url) {
  std::size_t urlSize = utils::countUrlSize(url);
  this->priorityQueue->push(UrlEntry(url, urlSize));
}

std::string PriorityPageScheduler::pop() {
  utils::assertTrue(
      !this->empty(),
      "Error(PriorityPageScheduler): pop when scheduler is empty");
  UrlEntry urlEntry = this->priorityQueue->top();
  this->priorityQueue->pop();
  return urlEntry.getUrl();
}

std::size_t PriorityPageScheduler::size() {
  return this->priorityQueue->size();
}

bool PriorityPageScheduler::empty(){
  return this->priorityQueue->empty();
}

} // namespace search_engine
