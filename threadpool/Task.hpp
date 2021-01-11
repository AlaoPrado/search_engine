#pragma once

namespace search_engine {

class Task {
public:
  Task();
  virtual ~Task();
  virtual void run() = 0;
};

} // namespace search_engine
