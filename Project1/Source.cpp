#include <AncientArcher/AncientArcher.h>
using namespace AA;
int main()
{
	static int ourcam = Engine->AddCamera(Engine->GetWindowWidth(), Engine->GetWindowHeight());

	const std::string skyboxfolder = "E:\\storage\\source\\repos\\AncientArcher\\AAUnitTest\\res\\skybox\\";
	const std::string order[6] = { "posx", "negx", "posy", "negy", "posz", "negz" };
	const std::string skyboxfileext = ".jpg";
	std::vector<std::string> cubemapfiles;
	for (int j = 0; j < 6; ++j)
	{
		cubemapfiles.emplace_back(skyboxfolder + order[j] + skyboxfileext);
	}
	const std::shared_ptr<Skybox> skybox = std::make_shared<Skybox>(cubemapfiles);
	Engine->SetSkybox(skybox);

	Engine->AddToDeltaUpdate([](float dt) {
		Engine->GetCamera(ourcam).ShiftYawAndPitch(5*dt, 0);
		});
	Engine->Run();
}