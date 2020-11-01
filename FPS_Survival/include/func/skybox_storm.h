#pragma once

#include "../../../AAEngine/include/Loop.h"


void setStormySkybox() {
	const std::string skyboxfolder = "..\\assets\\skyboxes\\";
	const std::string order[6] = { "right", "left", "up", "down", "front", "back" };
	const std::string skyboxtype = "stormydays\\";
	const std::string skyboxfileext = ".tga";
	std::vector<std::string> cubemapfiles;
	for (int j = 0; j < 6; ++j)
	{
		cubemapfiles.emplace_back(skyboxfolder + skyboxtype + order[j] + skyboxfileext);
	}
	const std::shared_ptr<AA::Skybox> skybox = std::make_shared<AA::Skybox>(cubemapfiles);
	LOOP->setSkybox(skybox);
}