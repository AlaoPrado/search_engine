#include "HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include <fstream>

namespace search_engine {

std::string HtmlParser::cleantext(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             node->v.element.tag != GUMBO_TAG_SCRIPT &&
             node->v.element.tag != GUMBO_TAG_STYLE) {

    std::string childrenText = "";
    GumboVector *children = &node->v.element.children;

    for (std::size_t i = 0; i < children->length; ++i) {
      const std::string childText = cleantext((GumboNode *)children->data[i]);

      if (!childText.empty()) {
        childrenText += childText + " ";
      }
    }

    return childrenText;
  } else {
    return "";
  }
}

void HtmlParser::extractText(const std::string fileDirectory,
                             std::string &text) {
  std::ifstream file;

  file.open(fileDirectory);

  utils::assertTrue(file.is_open(),
                    "Error(HtmlParser2): failed to open file " + fileDirectory);

  std::string textToParse((std::istreambuf_iterator<char>(file)),
                          (std::istreambuf_iterator<char>()));

  file.close();

  GumboOutput *output = gumbo_parse(textToParse.c_str());
  text = cleantext(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

} // namespace search_engine
