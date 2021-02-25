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
#include "DocumentCollection.hpp"
#include "../../utils/Assert.hpp"
#include "../../utils/File.hpp"
#include <exception>
#include <fstream>
#include <stdexcept>

namespace search_engine {

void DocumentCollection::readLineUrlAndId(std::string line, std::string &url,
                                          std::size_t &id) {
  const std::string separator = " ";
  std::size_t lastSeparatorposition = line.find_last_of(separator);

  utils::assertTrue(lastSeparatorposition != std::string::npos &&
                        lastSeparatorposition + 1 < line.length(),
                    "Error(DocumentCollection): invalid input format");

  url = line.substr(0, lastSeparatorposition);
  id = std::stoul(line.substr(lastSeparatorposition + 1));
}

DocumentCollection::DocumentCollection(std::string collectionDirectory) {
  const std::string indeFileName("index.txt");
  std::ifstream indexFile;

  indexFile.open(collectionDirectory + indeFileName);

  utils::assertTrue(indexFile.is_open(),
                    "Error(DocumentCollection): failed to open file");

  try {
    std::string line, url, documentDirectory;
    std::size_t id;
    while (std::getline(indexFile, line)) {
      this->readLineUrlAndId(line, url, id);
      documentDirectory = collectionDirectory + std::to_string(id) + ".html";
      this->documentList.push_back(Document(documentDirectory, url));
    }
  } catch (std::exception &e) {
    throw std::logic_error("Error(DocumentCollection): " +
                           std::string(e.what()));
  }
}

std::vector<Document> DocumentCollection::getDocumentList() {
  return this->documentList;
}

std::size_t DocumentCollection::getNumBytes() {
  std::size_t numBytes = 0;
  for (auto &&document : documentList) {
    numBytes += utils::fileSize(document.getDirectory());
  }
  return numBytes;
}

std::string DocumentCollection::toString() {
  std::string text;
  for (auto &&document : documentList) {
    text += document.getUrl() + ": " + document.getDirectory() + '\n';
  }
  return text;
}

} // namespace search_engine
