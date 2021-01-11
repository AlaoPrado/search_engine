#pragma once

#include "PageScheduler.hpp"
#include <map>

namespace search_engine {

class PageGroupScheduler : public PageScheduler {
private:
  std::size_t numPages;
  std::map<std::string, PageScheduler *> *pageGroupScheduler;
  std::map<std::string, bool> *pageGroupInWork;

public:
  PageGroupScheduler();
  ~PageGroupScheduler();
  virtual std::string pop() override;
  virtual void push(std::string url) override;
  virtual std::size_t size() override;
};

} // namespace search_engine
