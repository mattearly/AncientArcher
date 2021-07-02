#include "../include/AncientArcher/AncientArcher.h"
#include "Core.h"
#include "Scene/Camera.h"
#include "Scene/Prop.h"
#include "Scene/Lights.h"
#include "Renderer/ModelLoader.h"
#include "Settings/Settings.h"
#include "Settings/SettingsOptions.h"
#include "Sound/Speaker.h"
#include "Sound/SoundEffect.h"
#include "Sound/LongSound.h"
#include "GUI/PlainGUI.h"
#include "Utility/QueryShader.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <algorithm>
#include "GUI/imGUI.h"
#include "RenderingFramework.h"
#include <unordered_map>

#ifdef _DEBUG
#include <iostream>
#endif

namespace AA {

// Init, Run, Shutdown, Reset
void Init() {
  Core::Init();
}

i32 Run() {
  return Core::Run();
}

void Shutdown() noexcept {
  Core::Shutdown();
}

void SoftReset() noexcept {
  // process anything the user Requested and unload all meshes and textures
  teardown();

  mMusic = NULL;
  mCameras.clear();
  mProps.clear();
  onBegin.clear();
  onDeltaUpdate.clear();
  onKeyHandling.clear();
  onTimeoutKeyHandling.clear();
  onScrollHandling.clear();
  onMouseHandling.clear();
  onUpdate.clear();
  onSlowUpdate.clear();
  onTearDown.clear();

  mNonSpammableKeysTimeout = 0.f;
  mSlowUpdateTimeout = 0.f;
  mSlowUpdateWaitLength = .3337f;
  mNoSpamWaitLength = .5667f;

  SetMouseToNormal();
  SetMouseReadToNormal();

  //SetWindowClearColor();
}

void DragAndDropFiles(bool allowed) {
  auto tmp_settings = Settings::Get()->GetOptions();
  tmp_settings.drag_and_drop_files_support = allowed;
  Settings::Get()->SetOptions(tmp_settings);
}
// End Init, Run, Shutdown, Reset


// Camera
i32 AddCamera(i32 w, i32 h) {
  mCameras.emplace_back(w, h);
  return mCameras.back().GetUID();
}
i32 AddCamera() {
  mCameras.emplace_back();
  return mCameras.back().GetUID();
}
bool RemoveCamera(const i32 camId) {
  if (mCameras.empty())
    return false;

  auto before_size = mCameras.size();

  auto ret_it = mCameras.erase(
    std::remove_if(mCameras.begin(), mCameras.end(),
      [&](Camera c) {
    return c.GetUID() == camId;
  }),
    mCameras.end());

  auto after_size = mCameras.size();

  if (before_size != after_size)
    return true;  // success remove

  return false;   // fail remove
}
void SetCamMaxRenderDistance(i32 camId, f32 amt) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      cam.MaxRenderDistance = amt;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamToPerspective(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      cam.mProjectionType = ProjectionType::PERSPECTIVE;
      cam.updateProjectionMatrix();
      cam.updateViewMatrix();
      updateProjectionFromCam(mLitShader, cam);
      updateProjectionFromCam(mDiffShader, cam);
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamToOrtho(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      cam.mProjectionType = ProjectionType::ORTHO;
      cam.updateProjectionMatrix();
      cam.updateViewMatrix();
      updateProjectionFromCam(mLitShader, cam);
      updateProjectionFromCam(mDiffShader, cam);
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamFOV(i32 camId, f32 new_fov) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      if (cam.mProjectionType != ProjectionType::PERSPECTIVE)
        throw("changing FoV in ortho does nothing");
      cam.FOV = new_fov;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamDimensions(i32 camId, i32 w, i32 h) {
  if (w < MINSCREENWIDTH || h < MINSCREENHEIGHT ||
    w > MAXSCREENWIDTH || h > MAXSCREENHEIGHT)
    throw("invalid cam resize attempted");
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      cam.Width = w;
      cam.Height = h;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamPosition(i32 camId, glm::vec3 new_loc) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      cam.Position = new_loc;
      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamPitch(i32 camId, f32 new_pitch_degrees) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      if (new_pitch_degrees > 89.9f)
        new_pitch_degrees = 89.9f;
      else if (new_pitch_degrees < -89.9f)
        new_pitch_degrees = -89.9f;
      cam.Pitch = new_pitch_degrees;
      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamYaw(i32 camId, f32 new_yaw_degrees) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      if (new_yaw_degrees > 360.0f)
        new_yaw_degrees -= 360.f;
      else if (new_yaw_degrees < 0.f)
        new_yaw_degrees += 360.f;
      cam.Yaw = new_yaw_degrees;
      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void ShiftCamPosition(i32 camId, glm::vec3 offset) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      cam.Position += offset;
      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void ShiftCamPitchAndYaw(i32 camId, f64 pitch_offset_degrees, f64 yaw_offset_degrees) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      f64 new_pitch_degrees = cam.Pitch + pitch_offset_degrees;
      if (new_pitch_degrees > 89.9f)
        new_pitch_degrees = 89.9f;
      else if (new_pitch_degrees < -89.9f)
        new_pitch_degrees = -89.9f;
      cam.Pitch = static_cast<f32>(new_pitch_degrees);

      f64 new_yaw_degrees = cam.Yaw + yaw_offset_degrees;
      if (new_yaw_degrees > 360.0f)
        new_yaw_degrees -= 360.f;
      else if (new_yaw_degrees < 0.f)
        new_yaw_degrees += 360.f;
      cam.Yaw = static_cast<f32>(new_yaw_degrees);

      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
glm::vec3 GetCamFront(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return cam.Front;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
glm::vec3 GetCamRight(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return cam.Right;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
glm::vec3 GetCamPosition(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return cam.Position;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
f32 GetCamPitch(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return cam.Pitch;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
f32 GetCamYaw(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return cam.Yaw;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
vec2 GetPitchAndYaw(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return vec2(cam.Pitch, cam.Yaw);
    }
  }
  throw("cam id doesn't exist or is invalid");
}
mat4 GetProjectionMatrix(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      return cam.mProjectionMatrix;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
// End Camera


// 3d Game Objects
i32 AddProp(const char* path, i32 camId, SHADERTYPE shadertype) {
  switch (shadertype) {
  case SHADERTYPE::LIT:
    setupLitShader();
    break;
  case SHADERTYPE::DIFF:
    setupDiffShader();
    break;
  }

  mProps.emplace_back(path, camId, shadertype);

  return mProps.back().GetUID();
}
void SetPropTranslation(i32 propId, glm::vec3 new_pos) {
  for (auto& p : mProps) {
    if (p.GetUID() == propId) {
      p.translation = new_pos;
      p.mModelMatrixChanged = true;
      return;
    }
  }
  throw("prop id does not exist");
}
void SetPropScale(i32 propId, glm::vec3 new_scale) {
  for (auto& p : mProps) {
    if (p.GetUID() == propId) {
      p.scale = new_scale;
      p.mModelMatrixChanged = true;
      return;
    }
  }
  throw("prop id does not exist");
}
void SetPropRotationX(i32 propId, f32 new_x_rot) {
  for (auto& p : mProps) {
    if (p.GetUID() == propId) {
      p.eulerRotationX = new_x_rot;
      p.mModelMatrixChanged = true;
      return;
    }
  }
  throw("prop id does not exist");
}
void SetPropRotationY(i32 propId, f32 new_y_rot) {
  for (auto& p : mProps) {
    if (p.GetUID() == propId) {
      p.eulerRotationY = new_y_rot;
      p.mModelMatrixChanged = true;
      return;
    }
  }
  throw("prop id does not exist");
}
void SetPropRotationZ(i32 propId, f32 new_z_rot) {
  for (auto& p : mProps) {
    if (p.GetUID() == propId) {
      p.eulerRotationZ = new_z_rot;
      p.mModelMatrixChanged = true;
      return;
    }
  }
  throw("prop id does not exist");
}
// End 3d Game Objects


// Anim prop
//i32 AddAnimProp(const char* path, i32 camId, SHADERTYPE shadertype) {
//  setupAnimShader();
//  mAnimProps.emplace_back(path, camId, shadertype);
//  return mAnimProps.back().GetUID();
//}
//void SetAnimation(i32 anim_prop_id, const char* path) {
//  for (auto& ap : mAnimProps) {
//    if (ap.GetUID() == anim_prop_id) {
//      ap.SetAnimation(path);
//      return;
//    }
//  }
//  throw("anim prop not found");
//}
//void UpdateAnimation(i32 anim_prop_id, f32 dt) {
//  for (auto& ap : mAnimProps) {
//    if (ap.GetUID() == anim_prop_id) {
//      ap.UpdateAnimation(dt);
//      return;
//    }
//  }
//  throw("anim prop not found");
//}
// End Anim


// Basic Geometry
i32 AddPlane(const SHADERTYPE shadertype) {
  if (shadertype == SHADERTYPE::LIT)
    setupLitShader();
  else if (shadertype==SHADERTYPE::DIFF)
    setupDiffShader();
  mPlanes.emplace_back();
  mPlanes.back().shadertype = shadertype;
  return mPlanes.back().GetUID();
}
// End Basic Geometry


// Skybox
void SetSkybox(std::vector<std::string> incomingSkymapFiles) noexcept {
  if (mSkybox)
    return;  // already set

  if (incomingSkymapFiles.size() != 6)
    return;  // invalid size for a skybox

  mSkybox = std::make_shared<Skybox>(incomingSkymapFiles);

}
// End Skybox



// Directional Ligts
void SetDirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) {
  if (!mLitShader)
    setupLitShader();

  if (!mDirectionalLight) {
    mDirectionalLight = new DirectionalLight(dir, amb, diff, spec);
  } else {
    mDirectionalLight->Direction = dir;
    mDirectionalLight->Ambient = amb;
    mDirectionalLight->Diffuse = diff;
    mDirectionalLight->Specular = spec;
  }

  {
    assert(mLitShader);
    mLitShader->Use();
    mLitShader->SetInt("isDirectionalLightOn", 1);
    mLitShader->SetVec3("directionalLight.Direction", mDirectionalLight->Direction);
    mLitShader->SetVec3("directionalLight.Ambient", mDirectionalLight->Ambient);
    mLitShader->SetVec3("directionalLight.Diffuse", mDirectionalLight->Diffuse);
    mLitShader->SetVec3("directionalLight.Specular", mDirectionalLight->Specular);
  }
}
void RemoveDirectionalLight() {
  assert(mLitShader);
  mLitShader->Use();
  mLitShader->SetInt("isDirectionalLightOn", 0);
  delete mDirectionalLight;
  mDirectionalLight = NULL;
}
// End Directional Light


// Poi32 Light
i32 AddPointLight(glm::vec3 pos, f32 constant, f32 linear, f32 quad, glm::vec3 amb,
  glm::vec3 diff, glm::vec3 spec) {
  if (mPointLights.size() >= MAXPOINTLIGHTS)
    throw("too many poi32 lights");

  if (!mLitShader)
    setupLitShader();

  mPointLights.emplace_back(PointLight(pos, constant, linear, quad, amb, diff, spec));
  std::size_t new_point_loc = mPointLights.size() - 1;

  // push changes to shader
  {
    std::string position, constant, linear, quadratic, ambient, diffuse, specular;
    constant = linear = quadratic = ambient = diffuse = specular = position = "pointLight[";
    std::stringstream ss;
    ss << new_point_loc;
    position += ss.str();
    constant += ss.str();
    linear += ss.str();
    quadratic += ss.str();
    ambient += ss.str();
    diffuse += ss.str();
    specular += ss.str();
    position += "].";
    constant += "].";
    linear += "].";
    quadratic += "].";
    ambient += "].";
    diffuse += "].";
    specular += "].";
    position += "Position";
    constant += "Constant";
    linear += "Linear";
    quadratic += "Quadratic";
    ambient += "Ambient";
    diffuse += "Diffuse";
    specular += "Specular";

    assert(mLitShader);
    mLitShader->Use(); // <- vs lies if u see green squigglies
    mLitShader->SetVec3(position, mPointLights.back().Position);
    mLitShader->SetFloat(constant, mPointLights.back().Constant);
    mLitShader->SetFloat(linear, mPointLights.back().Linear);
    mLitShader->SetFloat(quadratic, mPointLights.back().Quadratic);
    mLitShader->SetVec3(ambient, mPointLights.back().Ambient);
    mLitShader->SetVec3(diffuse, mPointLights.back().Diffuse);
    mLitShader->SetVec3(specular, mPointLights.back().Specular);
    mLitShader->SetInt("NUM_POINT_LIGHTS", static_cast<int>(new_point_loc + 1));
  }
  return mPointLights.back().id;  // unique id
}
bool RemovePointLight(i32 which_by_id) {
  // returns true if successfully removed the poi32 light, false otherwise
  if (mPointLights.empty())
    return false;

  auto before_size = mPointLights.size();

  auto ret_it = mPointLights.erase(
    std::remove_if(mPointLights.begin(), mPointLights.end(), [&](const PointLight sl) { return sl.id == which_by_id; }),
    mPointLights.end());

  auto after_size = mPointLights.size();

  if (before_size != after_size) {
    mLitShader->Use();
    mLitShader->SetInt("NUM_POINT_LIGHTS", static_cast<int>(after_size));

    // sync lights on shader after the change
    for (i32 i = 0; i < after_size; i++) {
      ChangePointLight(
        mPointLights[i].id,
        mPointLights[i].Position,
        mPointLights[i].Constant,
        mPointLights[i].Linear,
        mPointLights[i].Quadratic,
        mPointLights[i].Ambient,
        mPointLights[i].Diffuse,
        mPointLights[i].Specular
      );
    }
    return true;
  } else
    return false;
}
void MovePointLight(i32 which, glm::vec3 new_pos) {
  if (which < 0)
    throw("dont");

  i32 loc_in_vec = 0;
  for (auto& pl : mPointLights) {
    if (pl.id == which) {
      pl.Position = new_pos;
      std::stringstream ss;
      ss << loc_in_vec;
      std::string position = "pointLight[" + ss.str() + "].Position";
      mLitShader->Use();
      mLitShader->SetVec3(position.c_str(), pl.Position);
      return;
    }
    loc_in_vec++;
  }
  throw("u messed up");
}
void ChangePointLight(i32 which, glm::vec3 new_pos, f32 new_constant, f32 new_linear, f32 new_quad,
  glm::vec3 new_amb, glm::vec3 new_diff, glm::vec3 new_spec) {
  if (which < 0)
    throw("dont");

  i32 loc_in_vec = 0;
  for (auto& pl : mPointLights) {
    if (pl.id == which) {
      // push changes to shader
      {
        pl.Position = new_pos;
        pl.Ambient = new_amb;
        pl.Constant = new_constant;
        pl.Diffuse = new_diff;
        pl.Linear = new_linear;
        pl.Quadratic = new_quad;
        pl.Specular = new_spec;
        std::string pos, ambient, constant, diffuse, linear, quadrat, specular;
        ambient = constant = diffuse = linear = quadrat = specular = pos = "pointLight[";
        std::stringstream ss;
        ss << loc_in_vec;
        pos += ss.str();
        constant += ss.str();
        linear += ss.str();
        quadrat += ss.str();
        ambient += ss.str();
        diffuse += ss.str();
        specular += ss.str();
        pos += "].";
        constant += "].";
        linear += "].";
        quadrat += "].";
        ambient += "].";
        diffuse += "].";
        specular += "].";
        pos += "Position";
        constant += "Constant";
        linear += "Linear";
        quadrat += "Quadratic";
        ambient += "Ambient";
        diffuse += "Diffuse";
        specular += "Specular";

        mLitShader->Use(); // <- vs lies if u see green squigglies
        mLitShader->SetVec3(pos, pl.Position);
        mLitShader->SetFloat(constant, pl.Constant);
        mLitShader->SetFloat(linear, pl.Linear);
        mLitShader->SetFloat(quadrat, pl.Quadratic);
        mLitShader->SetVec3(ambient, pl.Ambient);
        mLitShader->SetVec3(diffuse, pl.Diffuse);
        mLitShader->SetVec3(specular, pl.Specular);
      }
      return;
    }
    loc_in_vec++;
  }
  throw("u messed up");
}
// End Poi32 Light


// Spot Light
i32 AddSpotLight(glm::vec3 pos, glm::vec3 dir, f32 inner, f32 outer, f32 constant,
  f32 linear, f32 quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) {
  if (mSpotLights.size() == MAXSPOTLIGHTS) {
    throw("too many spot lights");
  }

  if (!mLitShader)
    setupLitShader();

  mSpotLights.emplace_back(SpotLight(pos, dir, inner, outer, constant, linear, quad, amb, diff, spec));
  auto new_spot_loc = mSpotLights.size() - 1;

  // push changes to shader
  {
    std::string pos, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
    ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = pos = "spotLight[";
    std::stringstream ss;
    ss << new_spot_loc;
    pos += ss.str();
    constant += ss.str();
    cutoff += ss.str();
    ocutoff += ss.str();
    direction += ss.str();
    linear += ss.str();
    quadrat += ss.str();
    ambient += ss.str();
    diffuse += ss.str();
    specular += ss.str();
    pos += "].";
    constant += "].";
    cutoff += "].";
    ocutoff += "].";
    direction += "].";
    linear += "].";
    quadrat += "].";
    ambient += "].";
    diffuse += "].";
    specular += "].";
    pos += "Position";
    constant += "Constant";
    cutoff += "CutOff";
    ocutoff += "OuterCutOff";
    direction += "Direction";
    linear += "Linear";
    quadrat += "Quadratic";
    ambient += "Ambient";
    diffuse += "Diffuse";
    specular += "Specular";

    assert(mLitShader);
    mLitShader->Use(); // <- vs lies if u see green squigglies
    mLitShader->SetVec3(pos, mSpotLights.back().Position);
    mLitShader->SetFloat(cutoff, mSpotLights.back().CutOff);
    mLitShader->SetFloat(ocutoff, mSpotLights.back().OuterCutOff);
    mLitShader->SetVec3(direction, mSpotLights.back().Direction);
    mLitShader->SetFloat(constant, mSpotLights.back().Constant);
    mLitShader->SetFloat(linear, mSpotLights.back().Linear);
    mLitShader->SetFloat(quadrat, mSpotLights.back().Quadratic);
    mLitShader->SetVec3(ambient, mSpotLights.back().Ambient);
    mLitShader->SetVec3(diffuse, mSpotLights.back().Diffuse);
    mLitShader->SetVec3(specular, mSpotLights.back().Specular);
    mLitShader->SetInt("NUM_SPOT_LIGHTS", static_cast<int>(new_spot_loc + 1));
  }
  return mSpotLights.back().id;  // unique id
}
bool RemoveSpotLight(i32 which_by_id) {
  // returns true if it removed the spot light, false otherwise
  if (mSpotLights.empty())
    return false;

  auto before_size = mSpotLights.size();

  /*auto ret_it = */mSpotLights.erase(
    std::remove_if(mSpotLights.begin(), mSpotLights.end(), [&](const SpotLight sl) { return sl.id == which_by_id; }),
    mSpotLights.end());

  auto after_size = mSpotLights.size();

  if (before_size != after_size) {
    mLitShader->Use();
    mLitShader->SetInt("NUM_SPOT_LIGHTS", static_cast<int>(after_size));

    // sync lights on shader after the change
    for (i32 i = 0; i < after_size; i++) {
      ChangeSpotLight(
        mSpotLights[i].id,
        mSpotLights[i].Position,
        mSpotLights[i].Direction,
        mSpotLights[i].CutOff,
        mSpotLights[i].OuterCutOff,
        mSpotLights[i].Constant,
        mSpotLights[i].Linear,
        mSpotLights[i].Quadratic,
        mSpotLights[i].Ambient,
        mSpotLights[i].Diffuse,
        mSpotLights[i].Specular
      );
    }
    return true;
  } else
    return false;
}
void MoveSpotLight(i32 which, glm::vec3 new_pos, glm::vec3 new_dir) {
  if (which < 0)
    throw("dont");

  i32 loc_in_vec = 0;
  for (auto& sl : mSpotLights) {
    if (sl.id == which) {
      sl.Position = new_pos;
      sl.Direction = new_dir;
      mLitShader->Use();
      std::stringstream ss;
      ss << loc_in_vec;
      std::string position = "spotLight[" + ss.str() + "].Position";
      std::string direction = "spotLight[" + ss.str() + "].Direction";
      mLitShader->SetVec3(position, sl.Position);
      mLitShader->SetVec3(direction, sl.Direction);
      return;
    }
    loc_in_vec++;
  }
  throw("u messed up");
}
void ChangeSpotLight(i32 which, glm::vec3 new_pos, glm::vec3 new_dir, f32 new_inner,
  f32 new_outer, f32 new_constant, f32 new_linear, f32 new_quad, glm::vec3 new_amb,
  glm::vec3 new_diff, glm::vec3 new_spec) {
  if (which < 0)
    throw("dont");

  i32 loc_in_vec = 0;
  for (auto& sl : mSpotLights) {
    if (sl.id == which) {
      // push changes to shader
      {
        sl.Position = new_pos;
        sl.Direction = new_dir;
        sl.Ambient = new_amb;
        sl.Constant = new_constant;
        sl.CutOff = new_inner;
        sl.OuterCutOff = new_outer;
        sl.Diffuse = new_diff;
        sl.Linear = new_linear;
        sl.Quadratic = new_quad;
        sl.Specular = new_spec;
        std::string pos, ambient, constant, cutoff, ocutoff, diffuse, direction, linear, quadrat, specular;
        ambient = constant = cutoff = ocutoff = diffuse = direction = linear = quadrat = specular = pos = "spotLight[";
        std::stringstream ss;
        ss << loc_in_vec;
        pos += ss.str();
        constant += ss.str();
        cutoff += ss.str();
        ocutoff += ss.str();
        direction += ss.str();
        linear += ss.str();
        quadrat += ss.str();
        ambient += ss.str();
        diffuse += ss.str();
        specular += ss.str();
        pos += "].";
        constant += "].";
        cutoff += "].";
        ocutoff += "].";
        direction += "].";
        linear += "].";
        quadrat += "].";
        ambient += "].";
        diffuse += "].";
        specular += "].";
        pos += "Position";
        constant += "Constant";
        cutoff += "CutOff";
        ocutoff += "OuterCutOff";
        direction += "Direction";
        linear += "Linear";
        quadrat += "Quadratic";
        ambient += "Ambient";
        diffuse += "Diffuse";
        specular += "Specular";

        mLitShader->Use(); // <- vs lies if u see green squigglies
        mLitShader->SetVec3(pos, sl.Position);
        mLitShader->SetFloat(cutoff, sl.CutOff);
        mLitShader->SetFloat(ocutoff, sl.OuterCutOff);
        mLitShader->SetVec3(direction, sl.Direction);
        mLitShader->SetFloat(constant, sl.Constant);
        mLitShader->SetFloat(linear, sl.Linear);
        mLitShader->SetFloat(quadrat, sl.Quadratic);
        mLitShader->SetVec3(ambient, sl.Ambient);
        mLitShader->SetVec3(diffuse, sl.Diffuse);
        mLitShader->SetVec3(specular, sl.Specular);
      }
      return;
    }
    loc_in_vec++;
  }
  throw("u messed up");
}
// End Spot Light


// Sound Effects
i32 AddSoundEffect(const char* path) {
  // make sure the sound effect hasn't already been cube_loaded
  for (const auto& pl : mSoundEffects) {
    if (path == pl->_FilePath.c_str())
      throw("sound from that path already loaded");
  }
  // Add a new sound effect
  mSoundEffects.reserve(mSoundEffects.size() + 1);
  mSoundEffects.push_back(new SoundEffect(path));

  // Add a new speaker
  mSpeakers.reserve(mSpeakers.size() + 1);
  mSpeakers.emplace_back(new Speaker());  //test, note, todo: same sound effects can be applied to multiple speakers

  // Associate the sound effect to the speaker.
  mSpeakers.back()->AssociateBuffer(mSoundEffects.back()->_Buffer);

  // return the unique ID of the speaker
  return mSpeakers.back()->GetUID();
}
void RemoveSoundEffect(i32 soundId) {
  if (mSoundEffects.empty())
    throw("no sounds exist, nothing to remove");

  auto before_size = mSoundEffects.size();

  auto after_size = mSoundEffects.size();

  if (before_size == after_size)
    throw("didn't remove anything");
}
void PlaySoundEffect(i32 id, bool interrupt) {
  if (mSpeakers.empty())
    throw("no speakers");

  for (auto& spkr : mSpeakers) {
    if (spkr->GetUID() == id) {
      if (interrupt) {
        spkr->PlayInterrupt();
        return;
      }
      spkr->PlayNoOverlap();
      return;
    }
  }
  throw("speaker not found");
}
// End Sound Effects


// Music
void AddMusic(const char* path) {
  if (!mMusic) {
    mMusic = new LongSound(path);
    return;
  }
  throw("music already loaded, use remove music first");
}
void RemoveMusic() {
  if (mMusic) {
    delete mMusic;
    mMusic = NULL;
    return;
  }
  throw("no music to remove");
}
void PlayMusic() {
  if (mMusic) {
    mMusic->Play();
    return;
  }
  throw("no music to play");
}
void PauseMusic() {
  if (mMusic) {
    mMusic->Pause();
    return;
  }
  throw("no music loaded");
}
void ResumeMusic() {
  if (mMusic) {
    mMusic->Resume();
    return;
  }
  throw("no music loaded");
}
void StopMusic() {
  if (mMusic) {
    mMusic->Stop();
    return;
  }
  throw("no music loaded");
}
void SetMusicVolume(f32 new_vol) {
  if (mMusic) {
    mMusic->SetVolume(new_vol);
    return;
  }
  throw("no music loaded");
}
// End Music


// Mouse
void SetMouseToHidden() noexcept { Core::SetMouseToHidden(); };
void SetMouseToDisabled() noexcept { Core::SetMouseToDisabled(); };
void SetMouseToNormal() noexcept { Core::SetMouseToNormal(); };
void SetMouseReadToFPP() noexcept { Core::SetMouseReadToFPP(); };
void SetMouseReadToNormal() noexcept { Core::SetMouseReadToNormal(); };
void SetMouseFPPSensitivity(f32 sensitivity) noexcept {
  mFPPMouseSensitivity = sensitivity;
}
f32 GetMouseFPPSensitivity() noexcept {
  return mFPPMouseSensitivity;
}
// End


// GUI
void AddButton(vec2 pos, vec2 scale, vec3 color, float alpha) {
  mGUI->AddButton(pos, scale.x, scale.y, color, alpha);
}
void AddButton(vec2 pos, vec2 scale, float alpha, const char* texture_path) {
  mGUI->AddButton(pos, scale.x, scale.y, alpha, texture_path);
}
void SetGUIVisibility(const bool value) {
  if (!mGUI)
    return;
  if (value) {
    mGUI->ShowInterface();
  } else {
    mGUI->HideInterface();
  }
}
// End GUI


// imGUI
void UseIMGUI(const bool value) {
  if (value == false) {
    if (mimGUI) {
      delete mimGUI;
      mimGUI = NULL;
    }
  } else if (value == true) {
    if (mimGUI) {
      return;
    } else {
      mimGUI = new imGUI();
      mimGUI->Init(mWindow, (char*)GL_NUM_SHADING_LANGUAGE_VERSIONS);
    }
  }
}
// End imGUI


// Window
void SetWindowClearColor(vec3 color) noexcept { Core::SetWindowClearColor(color); }
i32 GetWindowWidth() noexcept { return Core::GetWindowWidth(); }
i32 GetWindowHeight() noexcept { return Core::GetWindowHeight(); }
void SetWindowTitle(const char* name) noexcept { Core::SetWindowTitle(name); }
void ToggleFullscreen() noexcept {
  static bool to_fullscreen = true;
  if (to_fullscreen) {
    Core::SetWindowFullscreen(to_fullscreen);
  } else {
    Core::SetWindowFullscreen(to_fullscreen);
  }
  to_fullscreen = !to_fullscreen;
}
// End Window


// Loop Controls
void SetTimedOutKeyHandlingLength(const f32& newtime) {
  mNoSpamWaitLength = newtime;
}
void SetSlowUpdateTimeoutLength(const f32& newtime) {
  // !! warning, no checking, set at your own risk
  mSlowUpdateWaitLength = newtime;
}
u32 AddToOnBegin(void(*function)()) {
  static u32 next_begin_id = 0;
  next_begin_id++;
  onBegin.emplace(next_begin_id, function);
  return next_begin_id;
}
u32 AddToDeltaUpdate(void(*function)(f32)) {
  static u32 next_deltaupdate_id = 0;
  next_deltaupdate_id++;
  onDeltaUpdate.emplace(next_deltaupdate_id, function);
  return next_deltaupdate_id;
}
u32 AddToUpdate(void(*function)()) {
  static u32 next_update_id = 0;
  next_update_id++;
  onUpdate.emplace(next_update_id, function);
  return next_update_id;
}
u32 AddToSlowUpdate(void(*function)()) {
  static u32 next_slowupdate_id = 0;
  next_slowupdate_id++;
  onSlowUpdate.emplace(next_slowupdate_id, function);
  return next_slowupdate_id;
}
u32 AddToTimedOutKeyHandling(bool(*function)(KeyboardInput&)) {
  static u32 next_timedout_id = 0;
  next_timedout_id++;
  onTimeoutKeyHandling.emplace(next_timedout_id, function);
  return next_timedout_id;
}
u32 AddToScrollHandling(void(*function)(ScrollInput&)) {
  static u32 next_scrollhandling_id = 0;
  next_scrollhandling_id++;
  onScrollHandling.emplace(next_scrollhandling_id, function);
  return next_scrollhandling_id;
}
u32 AddToKeyHandling(void(*function)(KeyboardInput&)) {
  static u32 next_keyhandling_id = 0;
  next_keyhandling_id++;
  onKeyHandling.emplace(next_keyhandling_id, function);
  return next_keyhandling_id;
}
u32 AddToMouseHandling(void(*function)(MouseInput&)) {
  static u32 next_mousehandling_id = 0;
  next_mousehandling_id++;
  onMouseHandling.emplace(next_mousehandling_id, function);
  return next_mousehandling_id;
}
u32 AddToOnTeardown(void(*function)()) {
  static u32 next_teardown_id = 0;
  next_teardown_id++;
  onTearDown.emplace(next_teardown_id, function);
  return next_teardown_id;
}
bool RemoveFromOnBegin(u32 r_id) {
  return static_cast<bool>(onBegin.erase(r_id));
}
bool RemoveFromDeltaUpdate(u32 r_id) {
  return static_cast<bool>(onDeltaUpdate.erase(r_id));
}
bool RemoveFromUpdate(u32 r_id) {
  return static_cast<bool>(onUpdate.erase(r_id));
}
bool RemoveFromSlowUpdate(u32 r_id) {
  return static_cast<bool>(onSlowUpdate.erase(r_id));
}
bool RemoveFromTimedOutKeyHandling(u32 r_id) {
  return static_cast<bool>(onTimeoutKeyHandling.erase(r_id));
}
bool RemoveFromScrollHandling(u32 r_id) {
  return static_cast<bool>(onScrollHandling.erase(r_id));
}
bool RemoveFromKeyHandling(u32 r_id) {
  return static_cast<bool>(onKeyHandling.erase(r_id));
}
bool RemoveFromMouseHandling(u32 r_id) {
  return static_cast<bool>(onMouseHandling.erase(r_id));
}
bool RemoveFromTeardown(u32 r_id) {
  return static_cast<bool>(onTearDown.erase(r_id));
}

}  // end namespace AA
