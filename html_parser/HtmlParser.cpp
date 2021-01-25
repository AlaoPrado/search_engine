#include "HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include <fstream>

namespace search_engine {

void HtmlParser::readText(const std::string fileDirectory, std::string &text) {
  std::ifstream file;

  file.open(fileDirectory);

  utils::assertTrue(file.is_open(),
                    "Error(HtmlParser): failed to open file " + fileDirectory);

  std::string textToParse((std::istreambuf_iterator<char>(file)),
                          (std::istreambuf_iterator<char>()));

  text =
      "Esse é um texto de exemplo. Ele tem ponto, vírgula; também tem outros "
      "tipos de pontuação: como ponto e vírgula e dois pontos.";
}

} // namespace search_engine
