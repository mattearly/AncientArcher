/*
Filesystem
----------------------------------------------------------------------
Copyright (c) 2019-2020, Matthew Early matthewjearly@gmail.com
All rights reserved.
Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:
* Redistributions of source code must retain the above
	copyright notice, this list of conditions and the
	following disclaimer.
* Redistributions in binary form must reproduce the above
	copyright notice, this list of conditions and the
	following disclaimer in the documentation and/or other
	materials provided with the distribution.
* Neither the name of the Matthew Early, nor the names of its
	contributors may be used to endorse or promote products
	derived from this software without specific prior
	written permission of the assimp team.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------
*/
#include "Filesystem.h"
#include "LoadableAssets.h"
#include <iostream>

/**
 * Access all the file names in a folder (non-recursively) and return a vector full of all the paths.
 * Returns an empty vector if the path doesn't exist or if there are no items in the directory.
 */
std::vector<std::filesystem::path> cpp17_GetAllFileNamesInFolder(std::filesystem::path path)
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

std::vector<std::string> cpp17_GetAllFileNamesOfExtensionInFolder(std::filesystem::path path, const std::string& ext)
{
	std::filesystem::path demoFilePath = path;
	const std::size_t PATHSIZE = demoFilePath.string().size();

	std::string folderdir;
	if (std::filesystem::exists(demoFilePath))
	{
		folderdir = demoFilePath.string();
	}
	else
	{
		std::cout << "no path to " << path.c_str() << '\n';
		//return;  //todo: log error of non-existant path
	}

	// all the files in the path
	std::vector<std::filesystem::path> all_files = cpp17_GetAllFileNamesInFolder(demoFilePath);

	std::vector<std::string> outfiles;
	if (all_files.size() == 0)
	{
		std::cout << "no files in " << demoFilePath << '\n';
		//return;  //todo: log error
	}
	else  // attempt populate asset path to extension
	{
		std::string assetExtension = ext;
		for (const auto& filepath : all_files)
		{
			std::string filename = filepath.string().substr(PATHSIZE);
			if (filename.find(assetExtension) != std::string::npos)
			{
				outfiles.push_back(filename);
			}
		}
	}
	return outfiles;
}
