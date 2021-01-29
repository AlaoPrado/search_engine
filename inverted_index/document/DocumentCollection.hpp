#pragma once

#include "Document.hpp"
#include <cstddef>
#include <string>
#include <vector>

namespace search_engine {

class DocumentCollection {
private:
  std::vector<Document> documentList;

  void readLineUrlAndId(std::string line, std::string &url, std::size_t &id);

public:
  DocumentCollection(std::string collectionDirectory);
  std::vector<Document> getDocumentList();
  std::size_t getNumBytes();
  std::string toString();
};

} // namespace search_engine
