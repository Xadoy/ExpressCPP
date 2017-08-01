#include "utility.h"

void splitString(const std::string &s, const char delim, std::vector<std::string>& elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;

  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}
