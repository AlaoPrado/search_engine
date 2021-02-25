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
#include "InvertedListEntry.hpp"

namespace search_engine {

InvertedListEntry::InvertedListEntry(
    std::size_t documentId, std::size_t numOccurences,
    std::vector<std::size_t> *ocurrenceListBegin)
    : documentId(documentId), numOccurences(numOccurences),
      ocurrenceListBegin(ocurrenceListBegin) {}

std::size_t InvertedListEntry::getDocumentId(){
  return this->documentId;
}

std::size_t InvertedListEntry::getNumOcurrences() {
  return this->numOccurences;
}

const std::size_t *InvertedListEntry::getOcurrenceListBegin() {
  return this->ocurrenceListBegin->data();
}


std::size_t InvertedListEntry::getNumBytes() {
  return sizeof(this->documentId) + sizeof(this->numOccurences) +
         sizeof(this->ocurrenceListBegin);
}

std::string InvertedListEntry::toString() {
  std::string s("");

  s += std::to_string(this->documentId) + " " +
       std::to_string(this->numOccurences) + " ";

  size_t i = 0;
  for (; i < this->numOccurences - 1; i++) {
    s += std::to_string(this->ocurrenceListBegin->at(i)) + " ";
  }
  if (i < this->numOccurences) {
    s += std::to_string(this->ocurrenceListBegin->at(i));
  }
  return s;
}

} // namespace search_engine
