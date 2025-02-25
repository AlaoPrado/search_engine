// Copyright 2013 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: jdtang@google.com (Jonathan Tang)
// Modified by: alanpradoaraujo (Alan Prado Araújo)
//
// Gets the cleantext of a page.

#include "HtmlParser.hpp"
#include "../utils/Assert.hpp"
#include <fstream>

namespace search_engine {

bool HtmlParser::isTagContentAvoidable(GumboTag tag) {
  return tag == GUMBO_TAG_LINK || tag == GUMBO_TAG_META ||
         tag == GUMBO_TAG_STYLE || tag == GUMBO_TAG_SCRIPT ||
         tag == GUMBO_TAG_NOSCRIPT || tag == GUMBO_TAG_SVG ||
         tag == GUMBO_TAG_UNKNOWN;
}

std::string HtmlParser::getTagSeparator(GumboTag tag) {
  if (tag == GUMBO_TAG_HEAD || tag == GUMBO_TAG_TITLE ||
      tag == GUMBO_TAG_NOSCRIPT || tag == GUMBO_TAG_TEMPLATE ||
      tag == GUMBO_TAG_BODY || tag == GUMBO_TAG_ARTICLE ||
      tag == GUMBO_TAG_SECTION || tag == GUMBO_TAG_NAV ||
      tag == GUMBO_TAG_ASIDE || tag == GUMBO_TAG_H1 || tag == GUMBO_TAG_H2 ||
      tag == GUMBO_TAG_H3 || tag == GUMBO_TAG_H4 || tag == GUMBO_TAG_H5 ||
      tag == GUMBO_TAG_H6 || tag == GUMBO_TAG_HGROUP ||
      tag == GUMBO_TAG_HEADER || tag == GUMBO_TAG_FOOTER ||
      tag == GUMBO_TAG_ADDRESS || tag == GUMBO_TAG_P || tag == GUMBO_TAG_HR ||
      tag == GUMBO_TAG_BLOCKQUOTE || tag == GUMBO_TAG_OL ||
      tag == GUMBO_TAG_UL || tag == GUMBO_TAG_LI || tag == GUMBO_TAG_DL ||
      tag == GUMBO_TAG_DT || tag == GUMBO_TAG_DD ||
      tag == GUMBO_TAG_FIGCAPTION || tag == GUMBO_TAG_MAIN ||
      tag == GUMBO_TAG_TABLE || tag == GUMBO_TAG_CAPTION ||
      tag == GUMBO_TAG_TR || tag == GUMBO_TAG_TD || tag == GUMBO_TAG_TH ||
      tag == GUMBO_TAG_LEGEND || tag == GUMBO_TAG_LABEL ||
      tag == GUMBO_TAG_BUTTON || tag == GUMBO_TAG_OPTION ||
      tag == GUMBO_TAG_TEXTAREA || tag == GUMBO_TAG_PROGRESS ||
      tag == GUMBO_TAG_METER || tag == GUMBO_TAG_DETAILS ||
      tag == GUMBO_TAG_SUMMARY || tag == GUMBO_TAG_MENUITEM) {
    return "\n";
  }

  return "";
}

std::string HtmlParser::cleanText(GumboNode *node) {
  if (node->type == GUMBO_NODE_TEXT) {
    std::string text(node->v.text.text);

    if (!text.empty()) {
      text += " ";
    }
    return text;
  } else if (node->type == GUMBO_NODE_ELEMENT &&
             !isTagContentAvoidable(node->v.element.tag)) {
    std::string childrenText("");
    GumboVector *children = &node->v.element.children;

    for (std::size_t i = 0; i < children->length; ++i) {
      const std::string childText = cleanText((GumboNode *)children->data[i]);

      childrenText += childText;
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

  text = "";
  GumboOutput *output = gumbo_parse(textToParse.c_str());

  if (output->root->type == GUMBO_NODE_ELEMENT) {
    text += cleanText(output->root);
  }

  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

} // namespace search_engine
