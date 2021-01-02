#pragma once

#include "MasterFlag.hpp"
#include <vector>

class Task {
protected:
  MasterFlag *masterFlag;

public:
  Task(MasterFlag *masterFlag);
  virtual ~Task();
  virtual void run() = 0;
};
