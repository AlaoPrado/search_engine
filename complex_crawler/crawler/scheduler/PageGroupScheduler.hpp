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
  virtual bool empty() override;
  virtual void finishWork(std::string url);
  virtual bool blocked();
};

} // namespace search_engine
