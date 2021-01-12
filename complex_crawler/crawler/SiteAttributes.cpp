#include "SiteAttributes.hpp"

namespace search_engine {

SiteAttributes::SiteAttributes()
    : totalTime(0), numPages(0), numPagesLevel1(0), totalPageSize(0) {}

void SiteAttributes::addToTotalTime(double timeMs) {
  this->totalTime += timeMs;
}

double SiteAttributes::getAverageTime() {
  if (this->numPages != 0)
    return this->totalTime / this->numPages;

  return -1;
}

int SiteAttributes::getNumPages() { return this->numPages; }

void SiteAttributes::addNumPages(int numPages) { this->numPages += numPages; }

int SiteAttributes::getNumPagesLeve1() { return this->numPagesLevel1; }

void SiteAttributes::addNumPagesLeve1(int numPagesLevel1) {
  this->numPagesLevel1 += numPagesLevel1;
}

void SiteAttributes::addToTotalPageSize(std::size_t pageSizeInBytes) {
  this->totalPageSize += pageSizeInBytes;
}

std::size_t SiteAttributes::getAveragePageSize() {
  return this->totalPageSize / this->numPages;
}

} // namespace search_engine
