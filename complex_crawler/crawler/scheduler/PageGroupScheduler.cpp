#include "PageGroupScheduler.hpp"
#include "../../../utils/Assert.hpp"
#include "../../../utils/Url.hpp"
#include "PageScheduler.hpp"
#include "SimplePageScheduler.hpp"
#include <iostream>
#include <map>

namespace search_engine {

std::map<std::string, PageScheduler *> *pageGroupScheduler;

PageGroupScheduler::PageGroupScheduler() : numPages(0) {
  this->pageGroupScheduler = new std::map<std::string, PageScheduler *>();
  this->pageGroupInWork = new std::map<std::string, bool>();
}

PageGroupScheduler::~PageGroupScheduler() {
  for (auto it = this->pageGroupScheduler->begin();
       it != this->pageGroupScheduler->end(); it++) {
    delete it->second;
  }
  delete this->pageGroupScheduler;
  delete this->pageGroupInWork;
}

std::string PageGroupScheduler::pop() {
  for (auto it = this->pageGroupInWork->begin();
       it != this->pageGroupInWork->end(); it++) {
    // std::cout << "PageGroupScheduler pop loop " + it->first << std::endl;
    if (!it->second &&
        !this->pageGroupScheduler->operator[](it->first)->empty()) {
      this->pageGroupInWork->operator[](it->first) = true;
      this->numPages--;
      // std::cout << "PageGroupScheduler pop " + it->first << std::endl;
      return this->pageGroupScheduler->operator[](it->first)->pop();
    }
  }
  return "";
}

void PageGroupScheduler::push(std::string url) {
  std::string baseUrl = utils::baseUrl(url);
  // std::cout << "PageGroupScheduler push " + baseUrl << std::endl;
  auto it = this->pageGroupScheduler->find(baseUrl);
  if (it == this->pageGroupScheduler->end()) {
    this->pageGroupScheduler->operator[](baseUrl) = new SimplePageScheduler();
    this->pageGroupInWork->operator[](baseUrl) = false;
    // std::cout << "PageGroupScheduler push " + baseUrl << std::endl;
  }
  this->pageGroupScheduler->operator[](baseUrl)->push(url);
  this->numPages++;
  for (auto it = this->pageGroupInWork->begin();
       it != this->pageGroupInWork->end(); it++) {
    // std::cout << "PageGroupScheduler keys " + it->first << std::endl;
  }
  // std::cout << "PageGroupScheduler keys end" << std::endl;
}

std::size_t PageGroupScheduler::size() { return this->numPages; }

bool PageGroupScheduler::empty() { return this->numPages == 0; }

void PageGroupScheduler::finishWork(std::string url) {
  std::string baseUrl = utils::baseUrl(url);
  // std::cout << "PageGroupScheduler finishWork " + baseUrl << std::endl;
  auto it = this->pageGroupInWork->find(baseUrl);

  utils::assertTrue(
      it != this->pageGroupInWork->end(),
      "Error(PageGroupScheduler): the input URL does not belong to any groups");

  this->pageGroupInWork->operator[](baseUrl) = false;
}

bool PageGroupScheduler::blocked() {
  for (auto it = this->pageGroupInWork->begin();
       it != this->pageGroupInWork->end(); it++) {
    // std::cout << "PageGroupScheduler pop blocked " + it->first << std::endl;
    if (!it->second &&
        !this->pageGroupScheduler->operator[](it->first)->empty()) {
      // std::cout << "PageGroupScheduler pop unblocked " + it->first << std::endl;
      return false;
    }
  }
  return true;
}

} // namespace search_engine
