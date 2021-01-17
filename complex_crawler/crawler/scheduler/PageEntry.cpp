#include "PageEntry.hpp"
#include "../Page.hpp"

namespace search_engine {

PageEntry::PageEntry(Page page, int size) : page(page), size(size) {}

Page PageEntry::getPage() { return this->page; }

int PageEntry::getSize() { return this->size; }

} // namespace search_engine
