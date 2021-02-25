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
#pragma once

#include <cstddef>
#include <map>
#include <string>
#include <vector>

namespace search_engine {

class TextParser {
private:
  TextParser();

  static void extractNextSeparator(const std::string text,
                                   std::size_t beginPosition, bool &isSpace,
                                   std::size_t &endPosition);

  static void extractNextWord(const std::string text, std::size_t beginPosition,
                              std::string &word, std::size_t &endPosition);

  static void extractNextOccurence(
      const std::string text, std::size_t &textPostion,
      std::map<std::string, std::vector<std::size_t>> &occurenceListMap,
      std::size_t &occurencePositon, bool &succcess);

public:
  static void extractOccurenceListMap(
      const std::string text,
      std::map<std::string, std::vector<std::size_t>> &occuoccurenceListMaprenceMap);
};

} // namespace search_engine
