#include <stdexcept>
#include <string>
#include <iostream>

namespace search_engine { 
namespace utils {

void inline assertTrue(const bool condition, const std::string message){
  if(!condition){
    throw std::logic_error(message);
  }
}

}
}
