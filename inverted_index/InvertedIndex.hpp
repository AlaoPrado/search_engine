#pragma once

#include "InvertedList.hpp"
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
#include "document/Document.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class InvertedIndex {
private:
  std::map<std::string, InvertedList *> *invertedListMap;
  std::map<std::size_t, std::string> *urlMap;

  InvertedIndex();
  void
  addDocument(std::size_t documentId,
              std::map<std::string, std::vector<std::size_t>> &occurenceMap);
  void readLineIdAndUrl(std::string line, std::size_t &id, std::string &url);

public:
  InvertedIndex(std::vector<Document> &documentList);
  InvertedIndex(std::string fileName);
  ~InvertedIndex();
  std::size_t getInvertedListStructureNumBytes();
  std::size_t getUrlMapNumBytes();
  std::size_t getNumBytes();
  std::size_t getVocabularySize();
  std::string getVocabularyAverageOcurrenceString();
  std::size_t getAverageInvertedListSize();
  std::size_t getAverageInveredListNumOccurences();
  std::vector<std::string> getVocabulary();
  void store(std::string fileName);
  void search(std::string term, std::vector<std::string> &urlList);
};

} // namespace search_engine
