#include <iostream>
#include <sstream>
#include <string>
#include "bdb_tokens.hpp"

/*!
 * @brief tokenize string per delimiter character
 * @param s string to tokenize
 * @param delim delimiter, default tab
 * @return token vector
 */
std::vector<std::string> Bdb_tokens::tokenize(const std::string &s, const char delim) {
  std::vector<std::string> result;
  std::string::size_type from = 0;
  std::string::size_type stop = s.find(delim);
  while (stop != std::string::npos) {
    result.push_back(s.substr(from, stop - from));
    from = stop + 1;
    stop = s.find(delim, from);
  }
  result.push_back(s.substr(from));
  return result;
}

/**
 * @brief prints line with visible non-printables, includes number if count >= 0
 */
std::string Bdb_tokens::line_print(int count, const std::string &str) {
  std::ostringstream os;
  if (count > -1)
    os << count << ": ";
  for (const char &c: str) {
    if (c == '\t') os << "<tab>";
    else if (c == '\n') os << "<nl>";
    else os << c;
  }
  return os.str();
}
