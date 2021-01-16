#pragma once

#include <cstddef>

namespace search_engine {

class SiteAttributes {
private:
  double totalTime;
  int numPages, numPagesLevel1;
  std::size_t totalPageSize;

public:
  SiteAttributes();
  void addToTotalTime(double timeMs);
  double getAverageTime();
  int getNumPages();
  void addNumPages(int numPages);
  int getNumPagesLevel1();
  void addNumPagesLevel1(int numPagesLevel1);
  void addToTotalPageSize(std::size_t pageSizeInBytes);
  std::size_t getAveragePageSize();
};

} // namespace search_engine
