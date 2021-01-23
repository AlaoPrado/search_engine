#pragma once

#include <string>

namespace search_engine {

class Document {
private:
  std::string directory, url;

public:
  Document(std::string directory, std::string url);
  std::string getDirectory();
  std::string getUrl();
};

} // namespace search_engine
