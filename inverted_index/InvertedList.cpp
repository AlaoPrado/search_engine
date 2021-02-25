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
#include "InvertedList.hpp"
#include <iostream>

namespace search_engine {

InvertedList::InvertedList() {}

InvertedList::~InvertedList() {}

void InvertedList::add(std::size_t documentId, std::size_t numOccurences,
                       std::vector<std::size_t> &ocurrencePositionList) {

  this->ocurrencePositionListOfList.push_back(ocurrencePositionList);
  InvertedListEntry entry(documentId, numOccurences,
                          &(this->ocurrencePositionListOfList.back()));
  this->entriyList.push_back(entry);
}

InvertedListEntry *InvertedList::get(std::size_t index) {
  return &(this->entriyList.operator[](index));
}

std::size_t InvertedList::size() { return this->entriyList.size(); }

std::size_t InvertedList::getNumBytes() {
  std::size_t numBytes =
      sizeof(this->entriyList) + sizeof(this->ocurrencePositionListOfList);

  for (size_t i = 0; i < this->entriyList.size(); i++) {
    numBytes += this->entriyList[i].getNumBytes();
  }
  for (auto &&occurenceList : this->ocurrencePositionListOfList) {
    numBytes += occurenceList.size() * sizeof(std::size_t);
  }
  return numBytes;
}

std::size_t InvertedList::getNumOccurences() {
  std::size_t numOccurences = 0;

  for (auto &&occurenceList : this->ocurrencePositionListOfList) {
    numOccurences += occurenceList.size();
  }
  return numOccurences;
}

} // namespace search_engine
