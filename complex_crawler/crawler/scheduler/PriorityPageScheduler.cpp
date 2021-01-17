#include "PriorityPageScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include <string>
#include <vector>
namespace search_engine {

PageCompare::PageCompare() {}

bool PageCompare::operator()(PageEntry &x, PageEntry &y) const {
  return x.getUrlSize() > y.getUrlSize();
}

PriorityPageScheduler::PriorityPageScheduler() {
  this->priorityQueue =
      new std::priority_queue<PageEntry, std::vector<PageEntry>, PageCompare>();
}

PriorityPageScheduler::~PriorityPageScheduler() { delete this->priorityQueue; }

void PriorityPageScheduler::push(Page page) {
  std::size_t urlSize = utils::countUrlSize(page.getUrl());
  this->priorityQueue->push(PageEntry(page, urlSize));
}

Page PriorityPageScheduler::pop() {
  utils::assertTrue(
      !this->empty(),
      "Error(PriorityPageScheduler): pop when scheduler is empty");
  PageEntry pageEntry = this->priorityQueue->top();
  this->priorityQueue->pop();
  return pageEntry.getPage();
}

std::size_t PriorityPageScheduler::size() {
  return this->priorityQueue->size();
}

bool PriorityPageScheduler::empty(){
  return this->priorityQueue->empty();
}

} // namespace search_engine
