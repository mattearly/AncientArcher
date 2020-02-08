#pragma once
#include "../world/AAWorld.h"
#include <string>
#include <cassert>
#include <vector>
#include <memory>

void addSkybox(AAWorld& engine, const std::string& n)
{

	// this function is based off having skyboxes in an asset path with the name of the skybox being the folder name and the files within containg 

		//                                 up.png
		// skybox files: back.png left.png front.png right.png
		//                                 down.png


	int skybox_choice = 0;
	const int numOptions = 3;
	std::string name[numOptions] = { "nordic", "drakeq", "normandy" };

	if (n == "nordic")
	{
		skybox_choice = 0;
	}
	else if (n == "drakeq")
	{
		skybox_choice = 1;
	}
	else if (n == "normandy")
	{
		skybox_choice = 2;
	}
	assert(skybox_choice < numOptions);
	{
		// this is all based on where I am storing the data for cubemaps for testing
		std::string folderpath = "C:/Users/matt/Dropbox_me298414/Dropbox/SkyboxCubemaps/";  // my skybox asset path



		// my skybox file names
		std::string order[6] = { "/right", "/left", "/up", "/down", "/front", "/back" };

		std::string extension = ".png";
		std::vector<std::string> cubemapfiles;
		for (int j = 0; j < 6; ++j)
		{
			cubemapfiles.emplace_back(folderpath + name[skybox_choice] + order[j] + extension);
		}
		std::shared_ptr<AASkybox> skybox = std::make_shared<AASkybox>(cubemapfiles);
		engine.setSkybox(skybox);
	}
}