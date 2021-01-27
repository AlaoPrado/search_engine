#include "HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include <fstream>

namespace search_engine {

bool HtmlParser::isTagContentAvoidable(GumboTag tag) {
  return tag == GUMBO_TAG_SCRIPT || tag == GUMBO_TAG_STYLE ||
         tag == GUMBO_TAG_UNKNOWN;
}

std::string HtmlParser::getTagSeparator(GumboTag tag) {
  if (tag == GUMBO_TAG_P || tag == GUMBO_TAG_TITLE || tag == GUMBO_TAG_H1 ||
      tag == GUMBO_TAG_H2 || tag == GUMBO_TAG_H3 || tag == GUMBO_TAG_H4 ||
      tag == GUMBO_TAG_H5 || tag == GUMBO_TAG_H6) {
    return "\n";
  }

  return " ";
}

std::string HtmlParser::cleanText(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    return std::string(node->v.text.text);
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             !isTagContentAvoidable(node->v.element.tag)) {

    std::string childrenText = "";
    GumboVector *children = &node->v.element.children;

    for (std::size_t i = 0; i < children->length; ++i) {
      const std::string childText = cleanText((GumboNode *)children->data[i]);

      if (!childText.empty()) {
        childrenText += childText;
      }
    }
    if (!childrenText.empty()) {
      childrenText += getTagSeparator(node->v.element.tag);
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
  text = cleanText(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

} // namespace search_engine
