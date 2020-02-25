#include "Filesystem.h"
namespace mearly
{
/**
 * Access all the file names in a folder (non-recursively) and return a vector full of all the paths.
 * Returns an empty vector if the path doesn't exist or if there are no items in the directory.
 */
std::vector<std::filesystem::path> FileSystem::cpp17_GetAllFileNamesInFolder(std::filesystem::path path)
{
  std::vector<std::filesystem::path> fileNamePaths;

  if (!exists(path))
  {
    return fileNamePaths;  // no items within just a empty vector
  }

  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    fileNamePaths.push_back(entry);
  }

  return fileNamePaths;
}

}  // end namespace mearly
