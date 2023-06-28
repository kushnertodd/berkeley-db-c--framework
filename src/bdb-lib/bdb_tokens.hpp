#pragma once

#include <map>
#include <vector>

/*!
 * @class Bdb_tokens class to extract text line tokens
 * @details does not support embedded commas in comma-delimited tokens
 */
class Bdb_tokens {
 public:
  static std::string line_print(int count, const std::string &str);
  static std::vector<std::string> tokenize(const std::string &s, char delim);
};
