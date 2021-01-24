#include "Occurrence.hpp"

namespace search_engine {

Occurrence::Occurrence(std::string word, std::size_t position)
    : word(word), position(position) {}

std::string Occurrence::getWord() { return this->word; }

std::size_t Occurrence::getPosition() { return this->position; }

} // namespace search_engine
