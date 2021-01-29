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
