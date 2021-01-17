#pragma once

#include "../Page.hpp"
#include <string>

namespace search_engine {

class PageScheduler {
public:
  virtual ~PageScheduler(){};
  virtual Page pop() = 0;
  virtual void push(Page page) = 0;
  virtual std::size_t size() = 0;
  virtual bool empty() = 0;
};

} // namespace search_engine
