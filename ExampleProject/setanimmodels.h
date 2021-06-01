#pragma once
#include <AncientArcher/AncientArcher.h>
#include <AncientArcher/Types.h>
#include <AncientArcher/Utility/SceneCheck.h>
#include <AncientArcher/ShaderType.h>
bool animModelsSet = false;
void setanimmodels(int cam){
  if (animModelsSet)
    return;
  animModelsSet = true;
  int inherited_cam = cam;
  AA::printSceneHeaderInfo(AA::extractSceneInfo("E:\\AssetPack\\cyber_man.fbx"));
  AA::printSceneHeaderInfo(AA::extractSceneInfo("E:\\AssetPack\\Walking.fbx"));
  AA::AddToDeltaUpdate([](float dt) {
  });
}