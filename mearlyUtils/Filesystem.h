#pragma once
#include <string>
#include <vector>
#include <filesystem>
namespace mearly
{
class FileSystem
{
public:
  static std::vector<std::filesystem::path> cpp17_GetAllFileNamesInFolder(std::filesystem::path path);
};
} // end namespace mearly
