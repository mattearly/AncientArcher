#pragma once
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
namespace AA
{
class Files
{
public:
  static void ReadToString(const char* filepath, std::string& out) {
    std::ifstream input_file_stream(filepath);
    if (!input_file_stream.is_open())
      throw("invalid file path");
    std::stringstream data_stream;
    data_stream << input_file_stream.rdbuf();
    out = data_stream.str();
  }
  static std::string ReadToString(const char* filepath)
  {
    std::ifstream input_file_stream(filepath);
    if (!input_file_stream.is_open())
      throw("invalid file path");
    std::stringstream data_stream;
    data_stream << input_file_stream.rdbuf();
    return data_stream.str();
  }
private:
};
}  // end namespace AA