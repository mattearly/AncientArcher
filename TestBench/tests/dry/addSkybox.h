#pragma once
#include <iostream>
#include "../../world/AAWorld.h"
void addDemoSkybox(AAWorld& engine)
{

	// this function is based off having skyboxes in an asset path with the name of the skybox being the folder name and the files within containg 

		//                                 up.png
		// skybox files: back.png left.png front.png right.png
		//                                 down.png

	{
		//1 get the path ot the demo skyboxes
			// get path to demo files
		std::string thisFilePath = __FILE__;
#ifdef _WIN32
		thisFilePath.erase(thisFilePath.find_last_of("\\"));
		thisFilePath.erase(thisFilePath.find_last_of("\\"));
		thisFilePath.erase(thisFilePath.find_last_of("\\"));
		thisFilePath.erase(thisFilePath.find_last_of("\\") + 1);
		thisFilePath.append("assets\\drakeq\\");
#elif __linux__
		//todo
#else
		//todo
#endif

		std::cout << "demo skybox path: " << thisFilePath << '\n';

		// skybox file name order
		std::string order[6] = { "right", "left", "up", "down", "front", "back" };

		const std::string extension = ".png";

		std::vector<std::string> cubemapfiles;

		for (int j = 0; j < 6; ++j)
		{
			cubemapfiles.emplace_back(thisFilePath + order[j] + extension);
			std::cout << "\t" << j << ". " << cubemapfiles.at(j) << '\n';
		}

		std::cout << "cubemapfiles size: " << cubemapfiles.size() << '\n';
		
		
		if (cubemapfiles.size() != 6)
			return;


		const std::shared_ptr<AASkybox> skybox = std::make_shared<AASkybox>(cubemapfiles);
		
		
		engine.setSkybox(skybox);

	}
}