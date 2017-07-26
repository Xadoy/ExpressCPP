#include "route.h"
#include <iostream>
#include <algorithm>

using namespace CExpress;

const char delim = '/';
const char attriPlaceholder = ':';
const char querySign = '?';

Route::Route(RequestType t, const std::string & p) {
  // check for empty string, not start with slash or has a double slash
  if (p == "" || p[0] != delim || (p.size() > 1 && p.find("//") != std::string::npos)) {
    throw "Input path invalid.";
  }

  requestType = t;
  splitString(p, delim, pathTokens);
  pathTokens.erase(pathTokens.begin());
  if (pathTokens.size() > 0) {
    
    std::string & lastToken = pathTokens.back();

    int questionIndex = lastToken.find(querySign);
    if (questionIndex != std::string::npos) {
      lastToken = lastToken.substr(0, questionIndex);
    }
  }

  for (std::string & s : pathTokens) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  }
}

std::vector<std::string> Route::getPathTokens() const { return pathTokens; }

RequestType Route::getRequestType() const { return requestType; }

bool Route::matches(const Route& target) const {
  if (requestType != target.getRequestType()) {
    return false;
  }

  std::vector<std::string> targetTokens = target.getPathTokens();

  if (pathTokens.size() != targetTokens.size()) {
    return false;
  }

  for (int i = 0; i < (int)pathTokens.size(); ++i) {
    if ((pathTokens[i][0] == attriPlaceholder) != (targetTokens[i][0] == attriPlaceholder)) {
      continue;
    }

    if (pathTokens[i] != targetTokens[i]) {
      return false;
    }
  }

  return true;
}

void Route::splitString(const std::string &s, const char delim, std::vector<std::string>& elems) const {
  std::stringstream ss;
  ss.str(s);
  std::string item;

  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}