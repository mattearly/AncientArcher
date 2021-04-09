#include "../include/AncientArcher/AncientArcher.h"
#include "Scene/Camera.h"
#include "Scene/Prop.h"
#include "Scene/Lights.h"
#include "Renderer/ModelLoader.h"
#include "Renderer/OpenGL/OGLShader.h"
#include "Renderer/OpenGL/OGLGraphics.h"
#include "Controls/KeyboardInput.h"
#include "Controls/MouseInput.h"
#include "Controls/ScrollInput.h"
#include "Shader/LitShader.h"
#include "Shader/DiffShader.h"
#include "Settings/Settings.h"
#include "Settings/SettingsOptions.h"
#include "Sound/SoundDevice.h"
#include "Sound/Speaker.h"
#include "Sound/SoundEffect.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <unordered_map>

namespace AA
{
OGLShader* mLitShader;
OGLShader* mDiffShader;

const int MAXPOINTLIGHTS = 50;
const int MAXSPOTLIGHTS = 25;

float mNonSpammableKeysTimeout;  ///< keeps track of how long the keys have timed out
float mNoSpamWaitLength;         ///< how long the non-spammable keys are to time out for at least
float mSlowUpdateTimeout;        ///< keeps track of how how long the slow update has been timed out
float mSlowUpdateWaitLength;     ///< ms length the slow update times out for at least

DirectionalLight* mDirectionalLight; ///< directional light for lit shader(s)
std::vector<SpotLight>                      mSpotLights;       ///< array of current spot lights
std::vector<PointLight>                     mPointLights;      ///< array of current point lights
std::vector<Camera>                         mCameras;          ///< array of available cameras
std::vector<Prop>                           mProps;            ///< array of available objects
LongSound* mMusic;            ///< background music
std::vector<Speaker*>                        mSpeakers;         ///< array of places to play sound effects from
std::vector<SoundEffect*>                    mSoundEffects;     ///< array of ready speaker id to sound effects
std::shared_ptr<Skybox>                     mSkybox;           ///< the main skybox

std::unordered_map<uint32_t, std::function<void()> >               onBegin;               ///< list of functions to run once when runMainAncientArcher is called
std::unordered_map<uint32_t, std::function<void(float)> >          onDeltaUpdate;         ///< list of functions that rely on deltatime in the main AncientArcher
std::unordered_map<uint32_t, std::function<void()> >               onUpdate;              ///< list of functions that run every frame in the main AncientArcher
std::unordered_map<uint32_t, std::function<void()> >               onSlowUpdate;          ///< list of functions to run every only every mSlowUpdateWaitLength in the main AncientArcher
std::unordered_map<uint32_t, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling;  ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press
std::unordered_map<uint32_t, std::function<void(ScrollInput&)> >   onScrollHandling;      ///< list of functions to handle mouse wheel scroll every frame in the main AncientArcher
std::unordered_map<uint32_t, std::function<void(KeyboardInput&)> > onKeyHandling;         ///< list of functions to handle keypresses every frame in the main AncientArcher
std::unordered_map<uint32_t, std::function<void(MouseInput&)> >    onMouseHandling;       ///< list of functions to handle mouse movement every frame in the main AncientArcher
std::unordered_map<uint32_t, std::function<void()> >               onTearDown;            ///< list of functions to run when destroying

KeyboardInput mButtonState = {};
MouseInput    mMousePosition = {};
ScrollInput   mMouseWheelScroll = {};
bool mRenewFPP = true;
float mFPPMouseSensitivity = 0.1f;  ///< mouse sensitivity while in first person perspective

enum class STANDARDMOUSEZEROS { DEFAULT, TOP_LEFT_0_to_1, BOT_LEFT_0_to_1, TOP_LEFT_FULL_RANGE, BOT_LEFT_FULL_RANGE };
STANDARDMOUSEZEROS mStandardMouseZeros = STANDARDMOUSEZEROS::BOT_LEFT_0_to_1;

int NUM_POINT_LIGHTS = 0;
int NUM_SPOT_LIGHTS = 0;
bool isInit = false;
bool externWindowSizeDirty = true;
GLFWwindow* mWindow = nullptr;
const int MINSCREENWIDTH = 100;
const int MINSCREENHEIGHT = 100;
const int MAXSCREENWIDTH = 7680;  //8k
const int MAXSCREENHEIGHT = 4320;
MouseReporting mMouseReporting = MouseReporting::STANDARD;

bool isFPP() noexcept
{
  return (mMouseReporting == MouseReporting::PERSPECTIVE &&
    glfwGetInputMode(mWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);
}

LongSound& GetMusic()
{
  assert(mMusic);
  return *mMusic;
}

int AddCamera(int w, int h)
{
  mCameras.emplace_back(w, h);
  return mCameras.back().GetUID();
}

bool RemoveCamera(const int camId)
{
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

void setupLitShader()
{
  if (!mLitShader) {
    mLitShader = new OGLShader(lit_vert_src, lit_frag_src);
    std::cout << "Lit Shader is Live!\n";
  }
}

void setupDiffShader()
{
  if (!mDiffShader) {
    mDiffShader = new OGLShader(diff_vert_src, diff_frag_src);
    std::cout << "Diff Shader is Live!\n";
  }
}

int AddProp(const char* path, int camId, bool is_lit)
{
  if (is_lit)
    setupLitShader();
  else
    setupDiffShader();

  mProps.emplace_back(path, camId, is_lit);

  return mProps.back().GetUID();
}

void SetPropTranslation(int propId, glm::vec3 new_pos)
{
  for (auto& p : mProps)
  {
    if (p.GetUID() == propId)
    {
      p.translation = new_pos;
      p.updateFinalModelMatrix();
      return;
    }
  }
  throw("prop id does not exist");
}

void SetPropScale(int propId, glm::vec3 new_scale)
{
  for (auto& p : mProps)
  {
    if (p.GetUID() == propId)
    {
      p.scale = new_scale;
      p.updateFinalModelMatrix();
      return;
    }
  }
  throw("prop id does not exist");
}

void SetPropRotationY(int propId, float new_y_rot)
{
  for (auto& p : mProps)
  {
    if (p.GetUID() == propId)
    {
      p.eulerRotationY = new_y_rot;
      p.updateFinalModelMatrix();
      return;
    }
  }
  throw("prop id does not exist");
}

void SetDirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
  if (!mLitShader)
    setupLitShader();

  if (!mDirectionalLight)
  {
    mDirectionalLight = new DirectionalLight(dir, amb, diff, spec);
  }
  else
  {
    mDirectionalLight->Direction = dir;
    mDirectionalLight->Ambient = amb;
    mDirectionalLight->Diffuse = diff;
    mDirectionalLight->Specular = spec;
  }

  {
    assert(mLitShader);
    mLitShader->use();
    mLitShader->setInt("isDirectionalLightOn", 1);
    mLitShader->setVec3("directionalLight.Direction", mDirectionalLight->Direction);
    mLitShader->setVec3("directionalLight.Ambient", mDirectionalLight->Ambient);
    mLitShader->setVec3("directionalLight.Diffuse", mDirectionalLight->Diffuse);
    mLitShader->setVec3("directionalLight.Specular", mDirectionalLight->Specular);
  }
}

void RemoveDirectionalLight()
{
  assert(mLitShader);
  mLitShader->use();
  mLitShader->setInt("isDirectionalLightOn", 0);
  delete mDirectionalLight;
  mDirectionalLight = NULL;
}

// returns unique id assigned to this light
int AddPointLight(glm::vec3 pos, float constant, float linear, float quad, glm::vec3 amb,
  glm::vec3 diff, glm::vec3 spec)
{
  if (mPointLights.size() >= MAXPOINTLIGHTS)
    throw("too many point lights");

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
    mLitShader->use(); // <- vs lies if u see green squigglies
    mLitShader->setVec3(position, mPointLights.back().Position);
    mLitShader->setFloat(constant, mPointLights.back().Constant);
    mLitShader->setFloat(linear, mPointLights.back().Linear);
    mLitShader->setFloat(quadratic, mPointLights.back().Quadratic);
    mLitShader->setVec3(ambient, mPointLights.back().Ambient);
    mLitShader->setVec3(diffuse, mPointLights.back().Diffuse);
    mLitShader->setVec3(specular, mPointLights.back().Specular);
    mLitShader->setInt("NUM_POINT_LIGHTS", static_cast<int>(new_point_loc + 1));
  }

  return mPointLights.back().id;  // unique id

}

void MovePointLight(int which, glm::vec3 new_pos)
{
  if (which < 0)
    throw("dont");

  int loc_in_vec = 0;
  for (auto& pl : mPointLights)
  {
    if (pl.id == which)
    {
      pl.Position = new_pos;
      std::stringstream ss;
      ss << loc_in_vec;
      std::string position = "pointLight[" + ss.str() + "].Position";
      mLitShader->use();
      mLitShader->setVec3(position.c_str(), pl.Position);
      return;
    }
    loc_in_vec++;
  }

  throw("u messed up");
}

void ChangePointLight(int which, glm::vec3 new_pos, float new_constant, float new_linear, float new_quad,
  glm::vec3 new_amb, glm::vec3 new_diff, glm::vec3 new_spec)
{
  if (which < 0)
    throw("dont");

  int loc_in_vec = 0;
  for (auto& pl : mPointLights)
  {
    if (pl.id == which)
    {
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

        mLitShader->use(); // <- vs lies if u see green squigglies
        mLitShader->setVec3(pos, pl.Position);
        mLitShader->setFloat(constant, pl.Constant);
        mLitShader->setFloat(linear, pl.Linear);
        mLitShader->setFloat(quadrat, pl.Quadratic);
        mLitShader->setVec3(ambient, pl.Ambient);
        mLitShader->setVec3(diffuse, pl.Diffuse);
        mLitShader->setVec3(specular, pl.Specular);
      }
      return;
    }
    loc_in_vec++;
  }

  throw("u messed up");
}

// returns true if successfully removed the point light, false otherwise
bool RemovePointLight(int which_by_id)
{
  if (mPointLights.empty())
    return false;

  auto before_size = mPointLights.size();

  auto ret_it = mPointLights.erase(
    std::remove_if(mPointLights.begin(), mPointLights.end(), [&](const PointLight sl) { return sl.id == which_by_id; }),
    mPointLights.end());

  auto after_size = mPointLights.size();

  if (before_size != after_size)
  {
    mLitShader->use();
    mLitShader->setInt("NUM_POINT_LIGHTS", static_cast<int>(after_size));

    // sync lights on shader after the change
    for (int i = 0; i < after_size; i++)
    {
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
  }
  else
    return false;
}

// returns unique id assigned to this light
int AddSpotLight(glm::vec3 pos, glm::vec3 dir, float inner, float outer, float constant,
  float linear, float quad, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec)
{
  if (mSpotLights.size() == MAXSPOTLIGHTS)
  {
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
    mLitShader->use(); // <- vs lies if u see green squigglies
    mLitShader->setVec3(pos, mSpotLights.back().Position);
    mLitShader->setFloat(cutoff, mSpotLights.back().CutOff);
    mLitShader->setFloat(ocutoff, mSpotLights.back().OuterCutOff);
    mLitShader->setVec3(direction, mSpotLights.back().Direction);
    mLitShader->setFloat(constant, mSpotLights.back().Constant);
    mLitShader->setFloat(linear, mSpotLights.back().Linear);
    mLitShader->setFloat(quadrat, mSpotLights.back().Quadratic);
    mLitShader->setVec3(ambient, mSpotLights.back().Ambient);
    mLitShader->setVec3(diffuse, mSpotLights.back().Diffuse);
    mLitShader->setVec3(specular, mSpotLights.back().Specular);
    mLitShader->setInt("NUM_SPOT_LIGHTS", static_cast<int>(new_spot_loc + 1));
  }

  return mSpotLights.back().id;  // unique id
}

void ChangeSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir, float new_inner,
  float new_outer, float new_constant, float new_linear, float new_quad, glm::vec3 new_amb,
  glm::vec3 new_diff, glm::vec3 new_spec)
{
  if (which < 0)
    throw("dont");

  int loc_in_vec = 0;
  for (auto& sl : mSpotLights)
  {
    if (sl.id == which)
    {
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

        mLitShader->use(); // <- vs lies if u see green squigglies
        mLitShader->setVec3(pos, sl.Position);
        mLitShader->setFloat(cutoff, sl.CutOff);
        mLitShader->setFloat(ocutoff, sl.OuterCutOff);
        mLitShader->setVec3(direction, sl.Direction);
        mLitShader->setFloat(constant, sl.Constant);
        mLitShader->setFloat(linear, sl.Linear);
        mLitShader->setFloat(quadrat, sl.Quadratic);
        mLitShader->setVec3(ambient, sl.Ambient);
        mLitShader->setVec3(diffuse, sl.Diffuse);
        mLitShader->setVec3(specular, sl.Specular);
      }
      return;
    }
    loc_in_vec++;
  }

  throw("u messed up");
}

void MoveSpotLight(int which, glm::vec3 new_pos, glm::vec3 new_dir)
{
  if (which < 0)
    throw("dont");

  int loc_in_vec = 0;
  for (auto& sl : mSpotLights)
  {
    if (sl.id == which)
    {
      sl.Position = new_pos;
      sl.Direction = new_dir;
      mLitShader->use();
      std::stringstream ss;
      ss << loc_in_vec;
      std::string position = "spotLight[" + ss.str() + "].Position";
      std::string direction = "spotLight[" + ss.str() + "].Direction";
      mLitShader->setVec3(position.c_str(), sl.Position);
      mLitShader->setVec3(direction.c_str(), sl.Direction);
      return;
    }
    loc_in_vec++;
  }

  throw("u messed up");
}

// returns true if it removed the spot light, false otherwise
bool RemoveSpotLight(int which_by_id)
{
  if (mSpotLights.empty())
    return false;

  auto before_size = mSpotLights.size();

  /*auto ret_it = */mSpotLights.erase(
    std::remove_if(mSpotLights.begin(), mSpotLights.end(), [&](const SpotLight sl) { return sl.id == which_by_id; }),
    mSpotLights.end());

  auto after_size = mSpotLights.size();

  if (before_size != after_size)
  {
    mLitShader->use();
    mLitShader->setInt("NUM_SPOT_LIGHTS", static_cast<int>(after_size));

    // sync lights on shader after the change
    for (int i = 0; i < after_size; i++)
    {
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
  }
  else
    return false;
}

int AddSoundEffect(const char* path) {
  // make sure the sound effect hasn't already been loaded
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

void PlaySoundEffect(int id, bool interrupt) {
  if (mSpeakers.empty())
    throw("no speakers");

  for (auto& spkr : mSpeakers)
  {
    if (spkr->GetUID() == id) {
      if (interrupt) {
        spkr->PlayInterrupt();
        return;
      }
      spkr->PlayNoOverlap();
    }
  }

  throw("speaker not found");
}

void RemoveSoundEffect(int soundId)
{
  if (mSoundEffects.empty())
    throw("no sounds exist, nothing to remove");

  auto before_size = mSoundEffects.size();

  auto after_size = mSoundEffects.size();

  if (before_size == after_size)
    throw("didn't remove anything");
}

void ChangeMusic(const char* path)
{
  if (!mMusic)
    mMusic = new LongSound(path);
}

// todo: make a managed AddSkybox instead
void SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept
{
  mSkybox = skybox;
}

uint32_t AddToOnBegin(void(*function)())
{
  static uint32_t next_begin_id = 0;
  next_begin_id++;
  onBegin.emplace(next_begin_id, function);
  return next_begin_id;
}

uint32_t AddToDeltaUpdate(void(*function)(float))
{
  static uint32_t next_deltaupdate_id = 0;
  next_deltaupdate_id++;
  onDeltaUpdate.emplace(next_deltaupdate_id, function);
  return next_deltaupdate_id;
}

uint32_t AddToUpdate(void(*function)())
{
  static uint32_t next_update_id = 0;
  next_update_id++;
  onUpdate.emplace(next_update_id, function);
  return next_update_id;
}

uint32_t AddToSlowUpdate(void(*function)())
{
  static uint32_t next_slowupdate_id = 0;
  next_slowupdate_id++;
  onSlowUpdate.emplace(next_slowupdate_id, function);
  return next_slowupdate_id;
}

uint32_t AddToTimedOutKeyHandling(bool(*function)(KeyboardInput&))
{
  static uint32_t next_timedout_id = 0;
  next_timedout_id++;
  onTimeoutKeyHandling.emplace(next_timedout_id, function);
  return next_timedout_id;
}

uint32_t AddToScrollHandling(void(*function)(ScrollInput&))
{
  static uint32_t next_scrollhandling_id = 0;
  next_scrollhandling_id++;
  onScrollHandling.emplace(next_scrollhandling_id, function);
  return next_scrollhandling_id;
}

uint32_t AddToKeyHandling(void(*function)(KeyboardInput&))
{
  static uint32_t next_keyhandling_id = 0;
  next_keyhandling_id++;
  onKeyHandling.emplace(next_keyhandling_id, function);
  return next_keyhandling_id;
}

uint32_t AddToMouseHandling(void(*function)(MouseInput&))
{
  static uint32_t next_mousehandling_id = 0;
  next_mousehandling_id++;
  onMouseHandling.emplace(next_mousehandling_id, function);
  return next_mousehandling_id;
}

uint32_t AddToOnTeardown(void(*function)())
{
  static uint32_t next_teardown_id = 0;
  next_teardown_id++;
  onTearDown.emplace(next_teardown_id, function);
  return next_teardown_id;
}

bool RemoveFromOnBegin(uint32_t r_id)
{
  return static_cast<bool>(onBegin.erase(r_id));
}

bool RemoveFromDeltaUpdate(uint32_t r_id)
{
  return static_cast<bool>(onDeltaUpdate.erase(r_id));
}

bool RemoveFromUpdate(uint32_t r_id)
{
  return static_cast<bool>(onUpdate.erase(r_id));
}

bool RemoveFromSlowUpdate(uint32_t r_id)
{
  return static_cast<bool>(onSlowUpdate.erase(r_id));
}

bool RemoveFromTimedOutKeyHandling(uint32_t r_id)
{
  return static_cast<bool>(onTimeoutKeyHandling.erase(r_id));
}

bool RemoveFromScrollHandling(uint32_t r_id)
{
  return static_cast<bool>(onScrollHandling.erase(r_id));
}

bool RemoveFromKeyHandling(uint32_t r_id)
{
  return static_cast<bool>(onKeyHandling.erase(r_id));
}

bool RemoveFromMouseHandling(uint32_t r_id)
{
  return static_cast<bool>(onMouseHandling.erase(r_id));
}

bool RemoveFromTeardown(uint32_t r_id)
{
  return static_cast<bool>(onTearDown.erase(r_id));
}

void SetCursorToEnabled(bool isHardwareRendered)
{
  if (isHardwareRendered)
  {
    SetCursorToHidden();
  }
  else
  {
    SetCursorToVisible();
  }
}

void SetCamMaxRenderDistance(int camId, float amt)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      cam.MaxRenderDistance = amt;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void SetCamToPerspective(int camId)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      cam.RenderProjection = RenderProjection::PERSPECTIVE;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void SetCamFOV(int camId, float new_fov)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      if (cam.RenderProjection != RenderProjection::PERSPECTIVE)
        throw("changing FOV on wrong render projection");
      cam.FOV = new_fov;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void SetCamDimensions(int camId, int w, int h)
{
  if (w < MINSCREENWIDTH || h < MINSCREENHEIGHT ||
    w > MAXSCREENWIDTH || h > MAXSCREENHEIGHT)
    throw("invalid cam resize attempted");
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      cam.Width = w;
      cam.Height = h;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void SetCamPosition(int camId, glm::vec3 new_loc)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      cam.Position = new_loc;
      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void SetCamPitch(int camId, float new_pitch_degrees)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
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

void SetCamYaw(int camId, float new_yaw_degrees)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
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

void ShiftCamPosition(int camId, glm::vec3 offset)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      cam.Position += offset;
      cam.updateCameraVectors();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void ShiftCamPitchAndYaw(int camId, float pitch_offset_degrees, float yaw_offset_degrees)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      float new_pitch_degrees = cam.Pitch + pitch_offset_degrees;
      if (new_pitch_degrees > 89.9f)
        new_pitch_degrees = 89.9f;
      else if (new_pitch_degrees < -89.9f)
        new_pitch_degrees = -89.9f;
      cam.Pitch = new_pitch_degrees;

      float new_yaw_degrees = cam.Yaw + yaw_offset_degrees;
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

glm::vec3 GetCamFront(int camId)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      return cam.Front;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

glm::vec3 GetCamRight(int camId)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      return cam.Right;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

glm::vec3 GetCamPosition(int camId)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      return cam.Position;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

glm::mat4 GetProjectionMatrix(int camId)
{
  for (auto& cam : mCameras)
  {
    if (cam.GetUID() == camId)
    {
      glm::mat4 projection = glm::mat4(1);
      switch (cam.RenderProjection)
      {
      case RenderProjection::PERSPECTIVE:
      {
        float aspectRatio = static_cast<float>(cam.Width) / static_cast<float>(cam.Height);
        projection = glm::perspective(glm::radians(cam.FOV), aspectRatio, 0.0167f, cam.MaxRenderDistance);
      }
      break;
      case RenderProjection::ORTHO:
        projection = glm::ortho(
          0.f,
          static_cast<float>(cam.Width),
          0.f,
          static_cast<float>(cam.Height),
          0.0167f,
          cam.MaxRenderDistance
        );
        break;
      default:
        break;
      }
      return projection;
    }
  }
  throw("cam id doesn't exist or is invalid");
}

void SetSlowUpdateTimeoutLength(const float& newtime)
{
  // !! warning, no checking, set at your own risk
  mSlowUpdateWaitLength = newtime;
}

void keepWindowOpen() noexcept
{
  glfwSetWindowShouldClose(mWindow, 0);
}

bool isTryingToClose() noexcept
{
  return glfwWindowShouldClose(mWindow);
}

void closeWindow() noexcept
{
  glfwSetWindowShouldClose(mWindow, 1);
}

void pullButtonStateEvents()
{
  glfwPollEvents();

  // esc
  if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    mButtonState.esc = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
  {
    mButtonState.esc = false;
  }
  // function keys
  if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_PRESS)
  {
    mButtonState.f1 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_RELEASE)
  {
    mButtonState.f1 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_PRESS)
  {
    mButtonState.f2 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_RELEASE)
  {
    mButtonState.f2 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_PRESS)
  {
    mButtonState.f3 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_RELEASE)
  {
    mButtonState.f3 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_PRESS)
  {
    mButtonState.f4 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_RELEASE)
  {
    mButtonState.f4 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_PRESS)
  {
    mButtonState.f5 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_RELEASE)
  {
    mButtonState.f5 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_PRESS)
  {
    mButtonState.f6 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_RELEASE)
  {
    mButtonState.f6 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_PRESS)
  {
    mButtonState.f7 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_RELEASE)
  {
    mButtonState.f7 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_PRESS)
  {
    mButtonState.f8 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_RELEASE)
  {
    mButtonState.f8 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_PRESS)
  {
    mButtonState.f9 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_RELEASE)
  {
    mButtonState.f9 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_PRESS)
  {
    mButtonState.f10 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_RELEASE)
  {
    mButtonState.f10 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F11) == GLFW_PRESS)
  {
    mButtonState.f11 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F11) == GLFW_RELEASE)
  {
    mButtonState.f11 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F12) == GLFW_PRESS)
  {
    mButtonState.f12 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F12) == GLFW_RELEASE)
  {
    mButtonState.f12 = false;
  }
  // number key row
  if (glfwGetKey(mWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
  {
    mButtonState.graveAccent = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
  {
    mButtonState.graveAccent = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
  {
    mButtonState.n1 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_RELEASE)
  {
    mButtonState.n1 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS)
  {
    mButtonState.n2 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_RELEASE)
  {
    mButtonState.n2 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_PRESS)
  {
    mButtonState.n3 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_RELEASE)
  {
    mButtonState.n3 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_PRESS)
  {
    mButtonState.n4 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_4) == GLFW_RELEASE)
  {
    mButtonState.n4 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_PRESS)
  {
    mButtonState.n5 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_5) == GLFW_RELEASE)
  {
    mButtonState.n5 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_6) == GLFW_PRESS)
  {
    mButtonState.n6 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_6) == GLFW_RELEASE)
  {
    mButtonState.n6 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_7) == GLFW_PRESS)
  {
    mButtonState.n7 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_7) == GLFW_RELEASE)
  {
    mButtonState.n7 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_8) == GLFW_PRESS)
  {
    mButtonState.n8 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_8) == GLFW_RELEASE)
  {
    mButtonState.n8 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_9) == GLFW_PRESS)
  {
    mButtonState.n9 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_9) == GLFW_RELEASE)
  {
    mButtonState.n9 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_0) == GLFW_PRESS)
  {
    mButtonState.n0 = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_0) == GLFW_RELEASE)
  {
    mButtonState.n0 = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_MINUS) == GLFW_PRESS)
  {
    mButtonState.minus = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_MINUS) == GLFW_RELEASE)
  {
    mButtonState.minus = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_EQUAL) == GLFW_PRESS)
  {
    mButtonState.equal = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_EQUAL) == GLFW_RELEASE)
  {
    mButtonState.equal = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
  {
    mButtonState.backspace = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_BACKSPACE) == GLFW_RELEASE)
  {
    mButtonState.backspace = false;
  }
  // alphabet keys
  if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
  {
    mButtonState.a = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_RELEASE)
  {
    mButtonState.a = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_PRESS)
  {
    mButtonState.b = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_RELEASE)
  {
    mButtonState.b = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_PRESS)
  {
    mButtonState.c = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_RELEASE)
  {
    mButtonState.c = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
  {
    mButtonState.d = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_RELEASE)
  {
    mButtonState.d = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
  {
    mButtonState.e = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_RELEASE)
  {
    mButtonState.e = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_PRESS)
  {
    mButtonState.f = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_F) == GLFW_RELEASE)
  {
    mButtonState.f = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_G) == GLFW_PRESS)
  {
    mButtonState.g = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_G) == GLFW_RELEASE)
  {
    mButtonState.g = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_H) == GLFW_PRESS)
  {
    mButtonState.h = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_H) == GLFW_RELEASE)
  {
    mButtonState.h = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_I) == GLFW_PRESS)
  {
    mButtonState.i = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_I) == GLFW_RELEASE)
  {
    mButtonState.i = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_J) == GLFW_PRESS)
  {
    mButtonState.j = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_J) == GLFW_RELEASE)
  {
    mButtonState.j = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_K) == GLFW_PRESS)
  {
    mButtonState.k = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_K) == GLFW_RELEASE)
  {
    mButtonState.k = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_L) == GLFW_PRESS)
  {
    mButtonState.l = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_L) == GLFW_RELEASE)
  {
    mButtonState.l = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_PRESS)
  {
    mButtonState.m = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_RELEASE)
  {
    mButtonState.m = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_PRESS)
  {
    mButtonState.n = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_RELEASE)
  {
    mButtonState.n = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_PRESS)
  {
    mButtonState.o = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_O) == GLFW_RELEASE)
  {
    mButtonState.o = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_PRESS)
  {
    mButtonState.p = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_P) == GLFW_RELEASE)
  {
    mButtonState.p = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
  {
    mButtonState.q = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_RELEASE)
  {
    mButtonState.q = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_PRESS)
  {
    mButtonState.r = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_R) == GLFW_RELEASE)
  {
    mButtonState.r = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
  {
    mButtonState.s = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_RELEASE)
  {
    mButtonState.s = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_T) == GLFW_PRESS)
  {
    mButtonState.t = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_T) == GLFW_RELEASE)
  {
    mButtonState.t = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_U) == GLFW_PRESS)
  {
    mButtonState.u = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_U) == GLFW_RELEASE)
  {
    mButtonState.u = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_PRESS)
  {
    mButtonState.v = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_RELEASE)
  {
    mButtonState.v = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
  {
    mButtonState.w = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_RELEASE)
  {
    mButtonState.w = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
  {
    mButtonState.x = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_RELEASE)
  {
    mButtonState.x = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_PRESS)
  {
    mButtonState.y = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_Y) == GLFW_RELEASE)
  {
    mButtonState.y = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_PRESS)
  {
    mButtonState.z = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_RELEASE)
  {
    mButtonState.z = false;
  }
  // tab-shift-control-alt
  if (glfwGetKey(mWindow, GLFW_KEY_TAB) == GLFW_PRESS)
  {
    mButtonState.tab = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_TAB) == GLFW_RELEASE)
  {
    mButtonState.tab = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    mButtonState.leftShift = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
  {
    mButtonState.leftShift = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
  {
    mButtonState.rightShift = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE)
  {
    mButtonState.rightShift = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    mButtonState.leftControl = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
  {
    mButtonState.leftControl = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
  {
    mButtonState.rightControl = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) == GLFW_RELEASE)
  {
    mButtonState.rightControl = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
  {
    mButtonState.leftAlt = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
  {
    mButtonState.leftAlt = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
  {
    mButtonState.rightAlt = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_ALT) == GLFW_RELEASE)
  {
    mButtonState.rightAlt = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    mButtonState.spacebar = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
  {
    mButtonState.spacebar = false;
  }
  // brackets
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
  {
    mButtonState.leftSquareBracket = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_LEFT_BRACKET) == GLFW_RELEASE)
  {
    mButtonState.leftSquareBracket = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
  {
    mButtonState.rightSquareBracket = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_BRACKET) == GLFW_RELEASE)
  {
    mButtonState.rightSquareBracket = false;
  }
  // slash-quote-semicolon-enter
  if (glfwGetKey(mWindow, GLFW_KEY_BACKSLASH) == GLFW_PRESS)
  {
    mButtonState.backslash = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_BACKSLASH) == GLFW_RELEASE)
  {
    mButtonState.backslash = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
  {
    mButtonState.semiColon = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_SEMICOLON) == GLFW_RELEASE)
  {
    mButtonState.semiColon = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)
  {
    mButtonState.apostrophe = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_APOSTROPHE) == GLFW_RELEASE)
  {
    mButtonState.apostrophe = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
  {
    mButtonState.enter = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_ENTER) == GLFW_RELEASE)
  {
    mButtonState.enter = false;
  }
  // comma-period-forwardslash
  if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_PRESS)
  {
    mButtonState.comma = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_RELEASE)
  {
    mButtonState.comma = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_PERIOD) == GLFW_PRESS)
  {
    mButtonState.period = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_PERIOD) == GLFW_RELEASE)
  {
    mButtonState.period = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_SLASH) == GLFW_PRESS)
  {
    mButtonState.forwardSlash = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_SLASH) == GLFW_RELEASE)
  {
    mButtonState.forwardSlash = false;
  }
  // printscreen-etc
  if (glfwGetKey(mWindow, GLFW_KEY_PRINT_SCREEN) == GLFW_PRESS)
  {
    mButtonState.printScreen = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_PRINT_SCREEN) == GLFW_RELEASE)
  {
    mButtonState.printScreen = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_SCROLL_LOCK) == GLFW_PRESS)
  {
    mButtonState.scrollLock = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_SCROLL_LOCK) == GLFW_RELEASE)
  {
    mButtonState.scrollLock = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_PAUSE) == GLFW_PRESS)
  {
    mButtonState.pauseBreak = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_PAUSE) == GLFW_RELEASE)
  {
    mButtonState.pauseBreak = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_INSERT) == GLFW_PRESS)
  {
    mButtonState.insert = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_INSERT) == GLFW_RELEASE)
  {
    mButtonState.insert = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_DELETE) == GLFW_PRESS)
  {
    mButtonState.del = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_DELETE) == GLFW_RELEASE)
  {
    mButtonState.del = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_HOME) == GLFW_PRESS)
  {
    mButtonState.home = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_HOME) == GLFW_RELEASE)
  {
    mButtonState.home = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_END) == GLFW_PRESS)
  {
    mButtonState.end = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_END) == GLFW_RELEASE)
  {
    mButtonState.end = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_PAGE_UP) == GLFW_PRESS) {
    mButtonState.pageUp = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_PAGE_UP) == GLFW_RELEASE) {
    mButtonState.pageUp = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) {
    mButtonState.pageDown = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_PAGE_DOWN) == GLFW_RELEASE) {
    mButtonState.pageDown = false;
  }
  // arrows
  if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_PRESS) {
    mButtonState.upArrow = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_UP) == GLFW_RELEASE) {
    mButtonState.upArrow = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
    mButtonState.downArrow = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_DOWN) == GLFW_RELEASE) {
    mButtonState.downArrow = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_PRESS) {
    mButtonState.leftArrow = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_LEFT) == GLFW_RELEASE) {
    mButtonState.leftArrow = false;
  }
  if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    mButtonState.rightArrow = true;
  }
  else if (glfwGetKey(mWindow, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
    mButtonState.rightArrow = false;
  }

  // mouse clicks
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    mButtonState.mouseButton1 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
    mButtonState.mouseButton1 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
    mButtonState.mouseButton2 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
    mButtonState.mouseButton2 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
    mButtonState.mouseButton3 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
    mButtonState.mouseButton3 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_4) == GLFW_PRESS) {
    mButtonState.mouseButton4 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_4) == GLFW_RELEASE) {
    mButtonState.mouseButton4 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_5) == GLFW_PRESS) {
    mButtonState.mousebutton5 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_5) == GLFW_RELEASE) {
    mButtonState.mousebutton5 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_6) == GLFW_PRESS) {
    mButtonState.mouseButton6 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_6) == GLFW_RELEASE) {
    mButtonState.mouseButton6 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_7) == GLFW_PRESS) {
    mButtonState.mousebutton7 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_7) == GLFW_RELEASE) {
    mButtonState.mousebutton7 = false;
  }
  if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_8) == GLFW_PRESS) {
    mButtonState.mouseButton8 = true;
  }
  else if (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_8) == GLFW_RELEASE) {
    mButtonState.mouseButton8 = false;
  }
}

//void __setProjectionMatToAllShadersFromFirstCam_hack()
//{
//  if (mLitShader)
//  {
//    std::cout << "setting projection for lit shader\n";
//    mLitShader->use();
//    mLitShader->setMat4("projection", mCameras.front().Projection);
//  }
//
//  if (mDiffShader)
//  {
//    std::cout << "setting projection for diff shader\n";
//    mDiffShader->use();
//    mDiffShader->setMat4("projection", mCameras.front().Projection);
//  }
//
//  // if there is a skybox
//  if (mSkybox)
//  {
//    // if there is a camera
//    if (mCameras.size() > 0) {
//      // set the projection matrix on the skybox from the first cam proj matrix
//      mSkybox->setProjectionMatrix(mCameras.front());
//    }
//  }
//}

void begin()
{
  keepWindowOpen();

  for (const auto& oB : onBegin)
  {
    oB.second();
  }

  //__setProjectionMatToAllShadersFromFirstCam_hack();
}

void deltaUpdate()
{
  pullButtonStateEvents();

  // init delta clock on first tap into deltaUpdate
  static std::chrono::system_clock::time_point currTime;
  static std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();
  static std::chrono::duration<float> deltaTime;

  // update engine run delta times
  currTime = std::chrono::system_clock::now();
  deltaTime = currTime - lastTime;
  lastTime = currTime;

  // go through all updates that need access to delta time
  float elapsedTime = deltaTime.count();
  for (auto& oDU : onDeltaUpdate)
  {
    oDU.second(elapsedTime);
  }

  // process keyboard input
  for (auto& oKH : onKeyHandling) { oKH.second(mButtonState); }

  // absorb scroll wheel
  for (auto& oSH : onScrollHandling) { oSH.second(mMouseWheelScroll); }

  // reset scroll wheel to 0's after processing scroll wheel
  mMouseWheelScroll.xOffset = 0;
  mMouseWheelScroll.yOffset = 0;

  // handle mouse position
  for (auto& oMH : onMouseHandling) { oMH.second(mMousePosition); }

  // Snap cursor to the middle of the screen if it is in perspective and cursor is disabled (FPP mode)
  if (isFPP())
  {
    mMousePosition.xOffset = 0;
    mMousePosition.yOffset = 0;
  }

  // run through user prefered updates
  for (auto& oU : onUpdate) { oU.second(); }

  // delayed updates for things you don't want spammed.
  // update accum time for delayed updates
  mSlowUpdateTimeout += elapsedTime;
  // check to see if its time to process delayed updates
  if (mSlowUpdateTimeout > mSlowUpdateWaitLength)
  {
    // process all delayed updates
    for (auto& oSU : onSlowUpdate)
    {
      oSU.second();
    }

    mSlowUpdateTimeout = 0.f;
  }

  // needs updated, we'll use it in update with keyboard functions before the AncientArcher is done.
  // note that the keyboard processing cant be here because we have yet to run processSystemKeys()
  mNonSpammableKeysTimeout += elapsedTime;

  // only be executable after a timeout has been met, sort of like a cooldown
  if (mNonSpammableKeysTimeout > mNoSpamWaitLength)
  {
    // process unspammable keys
    for (auto& oTOK : onTimeoutKeyHandling)
    {
      // if we get a true we stop processing
      if (oTOK.second(mButtonState)) {
        //std::cout << "timeout key press detected. reseting timeoutkeytimer\n";
        mNonSpammableKeysTimeout = 0.f;
        break;
      }
    }
  }
}
void clearBackBuffer() noexcept
{
  OGLGraphics::ClearScreen();
}

void swapWindowBuffers() noexcept
{
  glfwSwapBuffers(mWindow);
}

//void __updateCamViewMatrices(int width, int height)
//{
//	for (auto& cam : mCameras)
//	{
//		cam.SetDimensions(width, height);
//	}
//
//	__setProjectionMatToAllShadersFromFirstCam_hack();
//}

void render()
{
  clearBackBuffer();

  if (externWindowSizeDirty) {
    if (mLitShader) {
      std::cout << "setting projection for lit shader on primary cam\n";
      mLitShader->use();
      mLitShader->setMat4("projection", mCameras.front().Projection);
    }

    if (mDiffShader)
    {
      std::cout << "setting projection for diff shader on primary cam\n";
      mDiffShader->use();
      mDiffShader->setMat4("projection", mCameras.front().Projection);
    }

    // if there is a skybox
    if (mSkybox)
    {
      // if there is a camera
      if (mCameras.size() > 0) {
        // set the projection matrix on the skybox from the first cam proj matrix
        mSkybox->setProjectionMatrix(mCameras.front());
      }
    }
    externWindowSizeDirty = false;
  }

  for (auto& p : mProps)  // todo: test const
  {
    // set view matrix
    // set the view matrix from the primary camera for each object is probably overkill
    if (p.mIsLit)
    {
      mLitShader->use();
      mLitShader->setMat4("view", mCameras.front().View);  // todo: hack
    }
    else
    {
      mDiffShader->use();
      mDiffShader->setMat4("view", mCameras.front().View);  // todo: hack
    }
    p.draw();
  }

  // draw skybox if one was specified
  if (mSkybox && !mCameras.empty()) { mSkybox->render(mCameras.front()); }

  swapWindowBuffers();
}

void teardown()
{
  // run user preferred functions first
  for (auto& oTD : onTearDown)
  {
    oTD.second();
  }
  // delete all the meshes and textures from GPU memory
  for (const auto& p : mProps)
  {
    ModelLoader::UnloadGameObject(p.mMeshes);  // todo: consider moving to the destructor the prop
  }

  for (const auto& spkr : mSpeakers)
  {
    delete spkr;
  }

  for (const auto& se : mSoundEffects)
  {
    delete se;
  }

  if (mLitShader) mLitShader->deleteShader();
  if (mDiffShader) mDiffShader->deleteShader();

}

void resetEngine() noexcept
{
  // process anything the user Requested and unload all meshes and textures
  teardown();

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
  mNoSpamWaitLength = .5667f;
  mSlowUpdateWaitLength = .3337f;

  SetCursorToEnabled();
  SetReadMouseCurorAsStandard();

  SetClearColor();
}

int GetWindowWidth() noexcept
{
  int width, height;

  glfwGetWindowSize(mWindow, &width, &height);

  return width;
}

int GetWindowHeight() noexcept
{
  int width, height;

  glfwGetWindowSize(mWindow, &width, &height);

  return height;
}

GLFWwindow* GetWindow() noexcept
{
  return mWindow;
}

void SetClearColor(glm::vec3 color)
{
  OGLGraphics::SetViewportClearColor(color);
}

void SetCursorToVisible() noexcept
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SetCursorToHidden() noexcept
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void SetCursorToDisabled() noexcept
{
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void SetWindowTitle(const char* name) noexcept
{
  glfwSetWindowTitle(mWindow, name);
}

// callback functions
void perspectiveMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  PerspectiveMouseHandler(static_cast<float>(xpos), static_cast<float>(ypos));
}
void standardMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  StandardMouseHandler(static_cast<float>(xpos), static_cast<float>(ypos));
}
void scrollCallback(GLFWwindow* window, double xpos, double ypos)
{
  ScrollHandler(static_cast<float>(xpos), static_cast<float>(ypos));
}
void reshapeCallback(GLFWwindow* window, int w, int h)
{
  ReshapeWindowHandler(w, h);
}

void SetReadMouseCurorAsFPP() noexcept
{
  mRenewFPP = true;
  ::glfwSetCursorPosCallback(mWindow, perspectiveMouseCallback);
  mMouseReporting = MouseReporting::PERSPECTIVE;
}

void SetReadMouseCurorAsStandard() noexcept
{
  ::glfwSetCursorPosCallback(mWindow, standardMouseCallback);
  mMouseReporting = MouseReporting::STANDARD;
}

void SetupScrollWheelCallback() noexcept
{
  ::glfwSetScrollCallback(mWindow, scrollCallback);
}

void SetupReshapeCallback() noexcept
{
  ::glfwSetFramebufferSizeCallback(mWindow, reshapeCallback);
}

void ReshapeWindowHandler(int width, int height)
{
  switch (Settings::Get()->GetOptions().renderer)
  {
  case RenderingFramework::OPENGL:
    OGLGraphics::SetViewportSize(0, 0, width, height);
    break;
  case RenderingFramework::D3D:
    break;
  case RenderingFramework::VULKAN:
    break;
  }

  // resize each camera
  for (auto& cam : mCameras)
  {
    cam.Width = static_cast<int>(width * cam.RatioToScreen.x);
    cam.Height = static_cast<int>(height * cam.RatioToScreen.y);
    cam.updateProjectionMatrix();
    std::cout << "projection updated for cam " << cam.GetUID() << '\n';
  }
  //TopLeftPositionOnScreen = glm::vec2(0.f);
  //RatioToScreen = glm::vec2(1);
  externWindowSizeDirty = true;
}

float GetMouseFPPSensitivity() noexcept
{
  return mFPPMouseSensitivity;
}

void SetMouseFPPSensitivity(float sensitivity) noexcept
{
  mFPPMouseSensitivity = sensitivity;
}

void resetControlVars() noexcept
{
  mFPPMouseSensitivity = 0.1f;
}

void perspectiveMouseMovement(float x, float y) noexcept
{
  float xOffset = 0, yOffset = 0;
  static float lastX, lastY;
  if (mRenewFPP)
  {
    mMousePosition.xOffset = 0;
    mMousePosition.yOffset = 0;
    lastX = x;
    lastY = y;
    mRenewFPP = false;
  }

  xOffset = x - lastX;
  yOffset = lastY - y;

  lastX = x;
  lastY = y;

  xOffset *= mFPPMouseSensitivity;
  yOffset *= mFPPMouseSensitivity;

  mMousePosition.xOffset = xOffset;
  mMousePosition.yOffset = yOffset;
}

/**
* Reports the mouse in x y space on the screen: bottom left should be 0,0
*/
void standardMouseMovement(float xpos, float ypos)
{
  float DisplayWindowWidth = static_cast<float>(GetWindowWidth());
  float DisplayWindowHeight = static_cast<float>(GetWindowHeight());

  switch (mStandardMouseZeros)
  {
  case STANDARDMOUSEZEROS::BOT_LEFT_0_to_1:
  {
    const float c_xpos = xpos / DisplayWindowWidth;
    const float c_ypos = -(ypos - DisplayWindowHeight) / DisplayWindowHeight;
    mMousePosition.xOffset = c_xpos;
    mMousePosition.yOffset = c_ypos;
  }
  break;
  case STANDARDMOUSEZEROS::TOP_LEFT_0_to_1:
  {
    const float c_xpos = xpos / DisplayWindowWidth;
    const float c_ypos = ypos / DisplayWindowHeight;
    mMousePosition.xOffset = c_xpos;
    mMousePosition.yOffset = c_ypos;
  }
  break;
  case STANDARDMOUSEZEROS::BOT_LEFT_FULL_RANGE:
  {
    const float c_xpos = xpos;
    const float c_ypos = -(ypos - DisplayWindowHeight);
    mMousePosition.xOffset = c_xpos;
    mMousePosition.yOffset = c_ypos;
  }
  break;
  case STANDARDMOUSEZEROS::TOP_LEFT_FULL_RANGE:
  {
    const float c_xpos = xpos;
    const float c_ypos = ypos;
    mMousePosition.xOffset = c_xpos;
    mMousePosition.yOffset = c_ypos;
  }
  break;
  }
}
void mouseScrollWheelMovement(float x, float y) noexcept
{
  mMouseWheelScroll.xOffset = x;
  mMouseWheelScroll.yOffset = y;
}

void InitEngine()
{
  if (!isInit) {
    SoundDevice::Init();
    mNonSpammableKeysTimeout = 0.f;
    mSlowUpdateTimeout = 0.f;
    mNoSpamWaitLength = .5667f;
    mSlowUpdateWaitLength = .3337f;
    mDiffShader = NULL;
    mLitShader = NULL;
    mMusic = NULL;
    mDirectionalLight = NULL;

    // set an error calback in case of failure we at least know
    static auto glfw_error_callback = [](int e, const char* msg) {
      if (e != 65543)
        throw("glfw callback error");
    };
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

    auto local_options = Settings::Get()->GetOptions();

    if (local_options.MSAA == true)
    {
      glfwWindowHint(GLFW_SAMPLES, local_options.msaa_samples);
    }

    if (local_options.renderer == RenderingFramework::OPENGL)
    {
      // with core profile, you have to create and manage your own VAO's, no default 
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      struct OpenGLVersion
      {
        OpenGLVersion() :major(-1), minor(-1) {}
        OpenGLVersion(int maj, int min) :major(maj), minor(min) {}
        int major = 0;
        int minor = 0;
      };
      // try more modern versions of OpenGL, don't use older than 4.3
      std::vector<OpenGLVersion> try_versions;
      //try_versions.push_back(OpenGLVersion(3, 3));
      //try_versions.push_back(OpenGLVersion(4, 0));
      //try_versions.push_back(OpenGLVersion(4, 1));
      //try_versions.push_back(OpenGLVersion(4, 2));
      try_versions.push_back(OpenGLVersion(4, 3));
      try_versions.push_back(OpenGLVersion(4, 4));
      try_versions.push_back(OpenGLVersion(4, 5));
      try_versions.push_back(OpenGLVersion(4, 6));

      while (!mWindow && !try_versions.empty())
      {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, try_versions.back().major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, try_versions.back().minor);
        mWindow = glfwCreateWindow(1280, 720, "AncientArcher Default Window Title", nullptr, nullptr);
        if (!mWindow) {
          try_versions.pop_back();
        }
        else  // save results to settings
        {
          local_options.RendererVersionMajor = try_versions.back().major;
          local_options.RendererVersionMinor = try_versions.back().minor;
        }
      }
    }

    if (!mWindow)
      throw("unsupported graphics");

    glfwMakeContextCurrent(mWindow);

    if (local_options.renderer == RenderingFramework::OPENGL)
    {
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
      {
        throw("Unable to context to OpenGL");
      }
      OGLGraphics::SetMSAA(local_options.MSAA);
    }

    // set all our options to what we set (mainly the major and minor version will be updated)
    Settings::Get()->SetOptions(local_options);

    SetupReshapeCallback();

    SetupScrollWheelCallback();

    SetReadMouseCurorAsStandard();

    SetClearColor();
  }
  isInit = true;
}

int Run()
{
  if (!isInit) {
    std::cout << "init first\n";
    return -4;
  }
  begin();
  while (!isTryingToClose())
  {
    deltaUpdate();
    render();
  }
  teardown();
  glfwTerminate();  //todo check if crash in debug still exists
  return 0;
}

void Shutdown() noexcept
{
  closeWindow();
}

void SoftReset()
{
  resetEngine();
}

void PerspectiveMouseHandler(float xpos, float ypos)
{
  perspectiveMouseMovement(xpos, ypos);
}

void StandardMouseHandler(float xpos, float ypos)
{
  standardMouseMovement(xpos, ypos);
}

void ScrollHandler(float xpos, float ypos)
{
  mouseScrollWheelMovement(xpos, ypos);
}

}  // end namespace AA