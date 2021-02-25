#pragma once

#include "InvertedListEntry.hpp"
#include <cstddef>
#include <list>
#include <vector>

/* Copyright 2021 Alan Prado Araújo

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
Author: alanpradoaraujo@gmail.com (Alan Prado Araújo) */
namespace search_engine {

class InvertedList {
private:
  std::vector<InvertedListEntry> entriyList;
  std::list<std::vector<size_t>> ocurrencePositionListOfList;

public:
  InvertedList();
  ~InvertedList();
  void add(std::size_t documentId, std::size_t numOccurences,
           std::vector<std::size_t> &ocurrencePositionList);
  InvertedListEntry *get(std::size_t index);
  std::size_t size();
  std::size_t getNumBytes();
  std::size_t getNumOccurences();
};

} // namespace search_engine
