#include "PageEntry.hpp"
#include "../Page.hpp"

namespace search_engine {

PageEntry::PageEntry(Page page, int urlSize) : page(page), urlSize(urlSize) {}

Page PageEntry::getPage() { return this->page; }

int PageEntry::getUrlSize() { return this->urlSize; }

} // namespace search_engine
