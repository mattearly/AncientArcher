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
#include "Sound/LongSound.h"
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
#include "GUI/PlainGUI.h"

namespace AA {
// Interanl Only (helpers, states, types, etc)
bool isEngineInit = false;
bool isWindowSizeDirty = true;  ///< true if proj matrices need re-adjusted for a new window size change
GLFWwindow* mWindow = nullptr;
const constexpr f32 mDefaultFPPMouseSensitivity = 0.1f;
f32 mFPPMouseSensitivity = mDefaultFPPMouseSensitivity;  ///< mouse sensitivity while in first person perspective
bool mSwitchedToFPP = false;
enum class MouseReporting { UNSET, STANDARD, PERSPECTIVE };
MouseReporting mMouseReporting = MouseReporting::UNSET;
OGLShader* mDiffShader = NULL;
OGLShader* mLitShader = NULL;
PlainGUI* mGUI = NULL;
void setupLitShader() {
  if (!mLitShader) {
    mLitShader = new OGLShader(lit_vert_src, lit_frag_src);
    std::cout << "Lit Shader is Live!\n";
  }
}
void setupDiffShader() {
  if (!mDiffShader) {
    mDiffShader = new OGLShader(diff_vert_src, diff_frag_src);
    std::cout << "Diff Shader is Live!\n";
  }
}
DirectionalLight* mDirectionalLight;         ///< directional light for lit shader(s)
i32 NUM_POINT_LIGHTS = 0;
const i32 MAXSPOTLIGHTS = 25;
std::vector<SpotLight>    mSpotLights;       ///< array of current spot lights
i32 NUM_SPOT_LIGHTS = 0;
const i32 MAXPOINTLIGHTS = 50;
std::vector<PointLight>   mPointLights;      ///< array of current poi32 lights
std::vector<Camera>       mCameras;          ///< array of available cameras
std::vector<Prop>         mProps;            ///< array of available objects
std::shared_ptr<Skybox>   mSkybox;           ///< the main skybox
std::vector<Speaker*>     mSpeakers;         ///< array of places to play sound effects from
std::vector<SoundEffect*> mSoundEffects;     ///< array of ready speaker id to sound effects
LongSound* mMusic;                           ///< background music
f32 mMusicRebufferCD = 0.f;
f32 mNonSpammableKeysTimeout;  ///< keeps track of how long the keys have timed out
f32 mNoSpamWaitLength;         ///< how long the non-spammable keys are to time out for at least
f32 mSlowUpdateTimeout;        ///< keeps track of how how long the slow update has been timed out
f32 mSlowUpdateWaitLength;     ///< ms length the slow update times out for at least
std::unordered_map<u32, std::function<void()> >               onBegin;               ///< list of functions to run once when runMainAncientArcher is called
std::unordered_map<u32, std::function<void(f32)> >            onDeltaUpdate;         ///< list of functions that rely on deltatime in the main AncientArcher
std::unordered_map<u32, std::function<void()> >               onUpdate;              ///< list of functions that run every frame in the main AncientArcher
std::unordered_map<u32, std::function<void()> >               onSlowUpdate;          ///< list of functions to run every only every mSlowUpdateWaitLength in the main AncientArcher
std::unordered_map<u32, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling;  ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press
std::unordered_map<u32, std::function<void(ScrollInput&)> >   onScrollHandling;      ///< list of functions to handle mouse wheel scroll every frame in the main AncientArcher
std::unordered_map<u32, std::function<void(KeyboardInput&)> > onKeyHandling;         ///< list of functions to handle keypresses every frame in the main AncientArcher
std::unordered_map<u32, std::function<void(MouseInput&)> >    onMouseHandling;       ///< list of functions to handle mouse movement every frame in the main AncientArcher
std::unordered_map<u32, std::function<void()> >               onTearDown;            ///< list of functions to run when destroying
KeyboardInput mButtonState = {};
MouseInput    mMousePosition = {};
ScrollInput   mMouseWheelScroll = {};
bool mNewKeyReads = false;
const i32 MINSCREENWIDTH = 100;
const i32 MINSCREENHEIGHT = 100;
const i32 MAXSCREENWIDTH = 7680;  //8k
const i32 MAXSCREENHEIGHT = 4320;
void keepWindowOpen() noexcept {
  glfwSetWindowShouldClose(mWindow, 0);
}
void begin() {
  keepWindowOpen();

  for (const auto& oB : onBegin) {
    oB.second();
  }

  //__setProjectionMatToAllShadersFromFirstCam_hack();
}
void update() {
  // init delta clock on first tap into update
  static std::chrono::system_clock::time_point currTime;
  static std::chrono::system_clock::time_point lastTime = std::chrono::system_clock::now();
  static std::chrono::duration<f32> deltaTime;

  // update engine run delta times
  currTime = std::chrono::system_clock::now();
  deltaTime = currTime - lastTime;
  lastTime = currTime;

  // go through all updates that need access to delta time
  f32 elapsedTime = deltaTime.count();

  // process keyboard input
  mNonSpammableKeysTimeout += elapsedTime;
  if (mNewKeyReads) {
    for (auto& oKH : onKeyHandling) { oKH.second(mButtonState); }
    // needs updated, we'll use it in update with keyboard functions before the AncientArcher is done.
    // note that the keyboard processing cant be here because we have yet to run processSystemKeys()
    // only be executable after a timeout has been met, sort of like a cooldown
    if (mNonSpammableKeysTimeout > mNoSpamWaitLength) {
      // process unspammable keys
      for (auto& oTOK : onTimeoutKeyHandling) {
        // if we get a true we stop processing
        if (oTOK.second(mButtonState)) {
          //std::cout << "timeout key press detected. reseting timeoutkeytimer\n";
          mNonSpammableKeysTimeout = 0.f;
          break;
        }
      }
    }
    mNewKeyReads = false;
  }

  for (auto& oDU : onDeltaUpdate) { oDU.second(elapsedTime); }

  // update accum time for delayed updates
  mSlowUpdateTimeout += elapsedTime;
  if (mSlowUpdateTimeout > mSlowUpdateWaitLength) {
    // process all delayed updates
    for (auto& oSU : onSlowUpdate) { oSU.second(); }
    mSlowUpdateTimeout = 0.f;
  }

  //if (mGUI)
    //mGUI->UpdateUniforms(vec2(GetWindowWidth(), GetWindowHeight()), vec2(mMousePosition.xOffset, mMousePosition.yOffset), elapsedTime);

  if (mMusic) {
    mMusicRebufferCD += elapsedTime;
    if (mMusicRebufferCD > .5f) {  // todo(maybe): math with file size and stuff to figure out how long this cd should actually be
      mMusic->UpdatePlayBuffer();
      mMusicRebufferCD = 0;
    }
  }
  // run through every frame update
  for (auto& oU : onUpdate) { oU.second(); }
}
void render() {
  OGLGraphics::ClearScreen();
  if (isWindowSizeDirty) {
    if (mLitShader) {
      std::cout << "setting projection for lit shader on primary cam\n";
      mLitShader->use();
      mLitShader->setMat4("projection", mCameras.front().Projection);
    }

    if (mDiffShader) {
      std::cout << "setting projection for diff shader on primary cam\n";
      mDiffShader->use();
      mDiffShader->setMat4("projection", mCameras.front().Projection);
    }

    // if there is a skybox
    if (mSkybox) {
      // if there is a camera
      if (mCameras.size() > 0) {
        // set the projection matrix on the skybox from the first cam proj matrix
        mSkybox->setProjectionMatrix(mCameras.front());
      }
    }

    isWindowSizeDirty = false;
  }

  for (auto& p : mProps)  // todo: test const
  {
    // set view matrix
    // set the view matrix from the primary camera for each object is probably overkill
    switch (p.mShaderType) {
    case SHADERTYPE::LIT:
      mLitShader->use();
      mLitShader->setMat4("view", mCameras.front().View);
      break;
    case SHADERTYPE::DIFF:
      mDiffShader->use();
      mDiffShader->setMat4("view", mCameras.front().View);
      break;
    }
    p.Draw();
  }

    p.draw();
  }

  // draw skybox if one was specified
  if (mSkybox && !mCameras.empty()) { mSkybox->render(mCameras.front()); }

  // if there is a gui
  if (mGUI) {
    mGUI->Draw();
  }

  glfwSwapBuffers(mWindow);
}
void teardown() {
  // run user preferred functions first
  for (auto& oTD : onTearDown) {
    oTD.second();
  }
  // delete all the meshes and textures from GPU memory
  for (const auto& p : mProps) {
    ModelLoader::UnloadGameObject(p.mMeshes);  // todo: consider moving to the destructor the prop
  }

  for (auto& spkr : mSpeakers) {
    delete spkr;
    spkr = NULL;
  }

  for (auto& se : mSoundEffects) {
    delete se;
    se = NULL;
  }

  if (mMusic) {
    delete mMusic;
    mMusic = NULL;
  }

  if (mLitShader) {
    delete mLitShader;
    mLitShader = NULL;
  }
  if (mDiffShader) {
    delete mLitShader;
    mDiffShader = NULL;
  }
}
// End Interanl Only


// Init, Run, Shutdown, Reset
void InitEngine() {
  if (!isEngineInit) {
    SoundDevice::Init();
    mNonSpammableKeysTimeout = 0.f;
    mSlowUpdateTimeout = 0.f;
    mNoSpamWaitLength = .1667f;
    mSlowUpdateWaitLength = .3337f;
    mDiffShader = NULL;
    mLitShader = NULL;
    mMusic = NULL;
    mDirectionalLight = NULL;

    // set an error calback in case of failure we at least know
    glfwSetErrorCallback([](i32 e, const char* msg) {
      if (e != 65543)
        throw("glfw callback error");
    });

    glfwInit();

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

    auto local_options = Settings::Get()->GetOptions();

    if (local_options.MSAA == true) {
      glfwWindowHint(GLFW_SAMPLES, local_options.msaa_samples);
    }

    if (local_options.renderer == RenderingFramework::OPENGL) {
      // with core profile, you have to create and manage your own VAO's, no default 
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
      struct OpenGLVersion {
        OpenGLVersion() :major(-1), minor(-1) {}
        OpenGLVersion(i32 maj, i32 min) :major(maj), minor(min) {}
        i32 major = 0;
        i32 minor = 0;
      };
      // try more modern versions of OpenGL, don't use older than 4.3
      std::vector<OpenGLVersion> try_versions;
      try_versions.push_back(OpenGLVersion(4, 3));
      try_versions.push_back(OpenGLVersion(4, 4));
      try_versions.push_back(OpenGLVersion(4, 5));
      try_versions.push_back(OpenGLVersion(4, 6));

      while (!mWindow && !try_versions.empty()) {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, try_versions.back().major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, try_versions.back().minor);
        mWindow = glfwCreateWindow(1280, 720, "AncientArcher Default Window Title", nullptr, nullptr);
        if (!mWindow) {
          try_versions.pop_back();
        } else  // save results to settings
        {
          local_options.RendererVersionMajor = try_versions.back().major;
          local_options.RendererVersionMinor = try_versions.back().minor;
        }
      }
    }

    if (!mWindow)
      throw("unsupported graphics");

    glfwSetWindowSizeLimits(mWindow, MINSCREENWIDTH, MINSCREENHEIGHT, MAXSCREENWIDTH, MAXSCREENHEIGHT);

    glfwMakeContextCurrent(mWindow);

    if (local_options.renderer == RenderingFramework::OPENGL) {

      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
      {
        throw("Unable to context to OpenGL");
      }
      OGLGraphics::SetMSAA(local_options.MSAA);
      OGLGraphics::SetBlend(true);
    }

    // set all our options to what we set (mainly the major and minor version will be updated)
    Settings::Get()->SetOptions(local_options);

    ::glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, i32 w, i32 h) {
      switch (Settings::Get()->GetOptions().renderer) {
      case RenderingFramework::OPENGL:
        OGLGraphics::SetViewportSize(0, 0, w, h);
        break;
      case RenderingFramework::D3D:
        break;
      case RenderingFramework::VULKAN:
        break;
      }
      for (auto& cam : mCameras) {
        cam.Width = static_cast<int>(w * cam.RatioToScreen.x);
        cam.Height = static_cast<int>(h * cam.RatioToScreen.y);
        cam.updateProjectionMatrix();
        std::cout << "projection updated for cam " << cam.GetUID() << '\n';
      }
      isWindowSizeDirty = true;
    });
    ::glfwSetScrollCallback(mWindow, [](GLFWwindow* w, f64 x, f64 y) {
      mMouseWheelScroll.xOffset = x;
      mMouseWheelScroll.yOffset = y;
      // process scroll wheel and reset back to 0
      for (const auto& oSH : onScrollHandling) { oSH.second(mMouseWheelScroll); }
      mMouseWheelScroll.yOffset = 0;
      mMouseWheelScroll.xOffset = 0;
    });
    ::glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* w, i32 button, i32 action, i32 mods) {
      // mouse clicks
      if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mButtonState.mouseButton1 = true;
      } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mButtonState.mouseButton1 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mButtonState.mouseButton2 = true;
      } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mButtonState.mouseButton2 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        mButtonState.mouseButton3 = true;
      } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        mButtonState.mouseButton3 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_4 && action == GLFW_PRESS) {
        mButtonState.mouseButton4 = true;
      } else if (button == GLFW_MOUSE_BUTTON_4 && action == GLFW_RELEASE) {
        mButtonState.mouseButton4 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_5 && action == GLFW_PRESS) {
        mButtonState.mousebutton5 = true;
      } else if (button == GLFW_MOUSE_BUTTON_5 && action == GLFW_RELEASE) {
        mButtonState.mousebutton5 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_6 && action == GLFW_PRESS) {
        mButtonState.mouseButton6 = true;
      } else if (button == GLFW_MOUSE_BUTTON_6 && action == GLFW_RELEASE) {
        mButtonState.mouseButton6 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_7 && action == GLFW_PRESS) {
        mButtonState.mousebutton7 = true;
      } else if (button == GLFW_MOUSE_BUTTON_7 && action == GLFW_RELEASE) {
        mButtonState.mousebutton7 = false;
      }
      if (button == GLFW_MOUSE_BUTTON_8 && action == GLFW_PRESS) {
        mButtonState.mouseButton8 = true;
      } else if (button == GLFW_MOUSE_BUTTON_8 && action == GLFW_RELEASE) {
        mButtonState.mouseButton8 = false;
      }
      mNewKeyReads = true;
    });
    ::glfwSetKeyCallback(mWindow, [](GLFWwindow* w, i32 key, i32 scancode, i32 action, i32 mods)
    {
      // esc
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        mButtonState.esc = true;
      } else if (key == GLFW_KEY_ESCAPE == GLFW_RELEASE) {
        mButtonState.esc = false;
      }
      // function keys
      if (key == GLFW_KEY_F1 == GLFW_PRESS) {
        mButtonState.f1 = true;
      } else if (key == GLFW_KEY_F1 == GLFW_RELEASE) {
        mButtonState.f1 = false;
      }
      if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        mButtonState.f2 = true;
      } else if (key == GLFW_KEY_F2 && action == GLFW_RELEASE) {
        mButtonState.f2 = false;
      }
      if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        mButtonState.f3 = true;
      } else if (key == GLFW_KEY_F3 && action == GLFW_RELEASE) {
        mButtonState.f3 = false;
      }
      if (key == GLFW_KEY_F4 && action == GLFW_PRESS) {
        mButtonState.f4 = true;
      } else if (key == GLFW_KEY_F4 && action == GLFW_RELEASE) {
        mButtonState.f4 = false;
      }
      if (key == GLFW_KEY_F5 && action == GLFW_PRESS) {
        mButtonState.f5 = true;
      } else if (key == GLFW_KEY_F5 && action == GLFW_RELEASE) {
        mButtonState.f5 = false;
      }
      if (key == GLFW_KEY_F6 && action == GLFW_PRESS) {
        mButtonState.f6 = true;
      } else if (key == GLFW_KEY_F6 && action == GLFW_RELEASE) {
        mButtonState.f6 = false;
      }
      if (key == GLFW_KEY_F7 && action == GLFW_PRESS) {
        mButtonState.f7 = true;
      } else if (key == GLFW_KEY_F7 && action == GLFW_RELEASE) {
        mButtonState.f7 = false;
      }
      if (key == GLFW_KEY_F8 && action == GLFW_PRESS) {
        mButtonState.f8 = true;
      } else if (key == GLFW_KEY_F8 && action == GLFW_RELEASE) {
        mButtonState.f8 = false;
      }
      if (key == GLFW_KEY_F9 && action == GLFW_PRESS) {
        mButtonState.f9 = true;
      } else if (key == GLFW_KEY_F9 && action == GLFW_RELEASE) {
        mButtonState.f9 = false;
      }
      if (key == GLFW_KEY_F10 && action == GLFW_PRESS) {
        mButtonState.f10 = true;
      } else if (key == GLFW_KEY_F10 && action == GLFW_RELEASE) {
        mButtonState.f10 = false;
      }
      if (key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        mButtonState.f11 = true;
      } else if (key == GLFW_KEY_F11 && action == GLFW_RELEASE) {
        mButtonState.f11 = false;
      }
      if (key == GLFW_KEY_F12 && action == GLFW_PRESS) {
        mButtonState.f12 = true;
      } else if (key == GLFW_KEY_F12 && action == GLFW_RELEASE) {
        mButtonState.f12 = false;
      }
      // number key row
      if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
        mButtonState.graveAccent = true;
      } else if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_RELEASE) {
        mButtonState.graveAccent = false;
      }
      if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        mButtonState.n1 = true;
      } else if (key == GLFW_KEY_1 && action == GLFW_RELEASE) {
        mButtonState.n1 = false;
      }
      if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        mButtonState.n2 = true;
      } else if (key == GLFW_KEY_2 && action == GLFW_RELEASE) {
        mButtonState.n2 = false;
      }
      if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        mButtonState.n3 = true;
      } else if (key == GLFW_KEY_3 && action == GLFW_RELEASE) {
        mButtonState.n3 = false;
      }
      if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        mButtonState.n4 = true;
      } else if (key == GLFW_KEY_4 && action == GLFW_RELEASE) {
        mButtonState.n4 = false;
      }
      if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        mButtonState.n5 = true;
      } else if (key == GLFW_KEY_5 && action == GLFW_RELEASE) {
        mButtonState.n5 = false;
      }
      if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        mButtonState.n6 = true;
      } else if (key == GLFW_KEY_6 && action == GLFW_RELEASE) {
        mButtonState.n6 = false;
      }
      if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
        mButtonState.n7 = true;
      } else if (key == GLFW_KEY_7 && action == GLFW_RELEASE) {
        mButtonState.n7 = false;
      }
      if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
        mButtonState.n8 = true;
      } else if (key == GLFW_KEY_8 && action == GLFW_RELEASE) {
        mButtonState.n8 = false;
      }
      if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
        mButtonState.n9 = true;
      } else if (key == GLFW_KEY_9 && action == GLFW_RELEASE) {
        mButtonState.n9 = false;
      }
      if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        mButtonState.n0 = true;
      } else if (key == GLFW_KEY_0 && action == GLFW_RELEASE) {
        mButtonState.n0 = false;
      }
      if (key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        mButtonState.minus = true;
      } else if (key == GLFW_KEY_MINUS && action == GLFW_RELEASE) {
        mButtonState.minus = false;
      }
      if (key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        mButtonState.equal = true;
      } else if (key == GLFW_KEY_EQUAL && action == GLFW_RELEASE) {
        mButtonState.equal = false;
      }
      if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
        mButtonState.backspace = true;
      } else if (key == GLFW_KEY_BACKSPACE && action == GLFW_RELEASE) {
        mButtonState.backspace = false;
      }
      // alphabet keys
      if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        mButtonState.a = true;
      } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        mButtonState.a = false;
      }
      if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        mButtonState.b = true;
      } else if (key == GLFW_KEY_B && action == GLFW_RELEASE) {
        mButtonState.b = false;
      }
      if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        mButtonState.c = true;
      } else if (key == GLFW_KEY_C && action == GLFW_RELEASE) {
        mButtonState.c = false;
      }
      if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        mButtonState.d = true;
      } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        mButtonState.d = false;
      }
      if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        mButtonState.e = true;
      } else if (key == GLFW_KEY_E && action == GLFW_RELEASE) {
        mButtonState.e = false;
      }
      if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        mButtonState.f = true;
      } else if (key == GLFW_KEY_F && action == GLFW_RELEASE) {
        mButtonState.f = false;
      }
      if (key == GLFW_KEY_G && action == GLFW_PRESS) {
        mButtonState.g = true;
      } else if (key == GLFW_KEY_G && action == GLFW_RELEASE) {
        mButtonState.g = false;
      }
      if (key == GLFW_KEY_H && action == GLFW_PRESS) {
        mButtonState.h = true;
      } else if (key == GLFW_KEY_H && action == GLFW_RELEASE) {
        mButtonState.h = false;
      }
      if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        mButtonState.i = true;
      } else if (key == GLFW_KEY_I && action == GLFW_RELEASE) {
        mButtonState.i = false;
      }
      if (key == GLFW_KEY_J && action == GLFW_PRESS) {
        mButtonState.j = true;
      } else if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
        mButtonState.j = false;
      }
      if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        mButtonState.k = true;
      } else if (key == GLFW_KEY_K && action == GLFW_RELEASE) {
        mButtonState.k = false;
      }
      if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        mButtonState.l = true;
      } else if (key == GLFW_KEY_L && action == GLFW_RELEASE) {
        mButtonState.l = false;
      }
      if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        mButtonState.m = true;
      } else if (key == GLFW_KEY_M && action == GLFW_RELEASE) {
        mButtonState.m = false;
      }
      if (key == GLFW_KEY_N && action == GLFW_PRESS) {
        mButtonState.n = true;
      } else if (key == GLFW_KEY_N && action == GLFW_RELEASE) {
        mButtonState.n = false;
      }
      if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        mButtonState.o = true;
      } else if (key == GLFW_KEY_O && action == GLFW_RELEASE) {
        mButtonState.o = false;
      }
      if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        mButtonState.p = true;
      } else if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
        mButtonState.p = false;
      }
      if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        mButtonState.q = true;
      } else if (key == GLFW_KEY_Q && action == GLFW_RELEASE) {
        mButtonState.q = false;
      }
      if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        mButtonState.r = true;
      } else if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
        mButtonState.r = false;
      }
      if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        mButtonState.s = true;
      } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        mButtonState.s = false;
      }
      if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        mButtonState.t = true;
      } else if (key == GLFW_KEY_T && action == GLFW_RELEASE) {
        mButtonState.t = false;
      }
      if (key == GLFW_KEY_U && action == GLFW_PRESS) {
        mButtonState.u = true;
      } else if (key == GLFW_KEY_U && action == GLFW_RELEASE) {
        mButtonState.u = false;
      }
      if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        mButtonState.v = true;
      } else if (key == GLFW_KEY_V && action == GLFW_RELEASE) {
        mButtonState.v = false;
      }
      if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        mButtonState.w = true;
      } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        mButtonState.w = false;
      }
      if (key == GLFW_KEY_X && action == GLFW_PRESS) {
        mButtonState.x = true;
      } else if (key == GLFW_KEY_X && action == GLFW_RELEASE) {
        mButtonState.x = false;
      }
      if (key == GLFW_KEY_Y && action == GLFW_PRESS) {
        mButtonState.y = true;
      } else if (key == GLFW_KEY_Y && action == GLFW_RELEASE) {
        mButtonState.y = false;
      }
      if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        mButtonState.z = true;
      } else if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
        mButtonState.z = false;
      }
      // tab-shift-control-alt
      if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        mButtonState.tab = true;
      } else if (key == GLFW_KEY_TAB && action == GLFW_RELEASE) {
        mButtonState.tab = false;
      }
      if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
        mButtonState.leftShift = true;
      } else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        mButtonState.leftShift = false;
      }
      if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_PRESS) {
        mButtonState.rightShift = true;
      } else if (key == GLFW_KEY_RIGHT_SHIFT && action == GLFW_RELEASE) {
        mButtonState.rightShift = false;
      }
      if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS) {
        mButtonState.leftControl = true;
      } else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
        mButtonState.leftControl = false;
      }
      if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_PRESS) {
        mButtonState.rightControl = true;
      } else if (key == GLFW_KEY_RIGHT_CONTROL && action == GLFW_RELEASE) {
        mButtonState.rightControl = false;
      }
      if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
        mButtonState.leftAlt = true;
      } else if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
        mButtonState.leftAlt = false;
      }
      if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS) {
        mButtonState.rightAlt = true;
      } else if (key == GLFW_KEY_RIGHT_ALT && action == GLFW_RELEASE) {
        mButtonState.rightAlt = false;
      }
      if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        mButtonState.spacebar = true;
      } else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        mButtonState.spacebar = false;
      }
      // brackets
      if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
        mButtonState.leftSquareBracket = true;
      } else if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_RELEASE) {
        mButtonState.leftSquareBracket = false;
      }
      if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
        mButtonState.rightSquareBracket = true;
      } else if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_RELEASE) {
        mButtonState.rightSquareBracket = false;
      }
      // slash-quote-semicolon-enter
      if (key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        mButtonState.backslash = true;
      } else if (key == GLFW_KEY_BACKSLASH && action == GLFW_RELEASE) {
        mButtonState.backslash = false;
      }
      if (key == GLFW_KEY_SEMICOLON && action == GLFW_PRESS) {
        mButtonState.semiColon = true;
      } else if (key == GLFW_KEY_SEMICOLON && action == GLFW_RELEASE) {
        mButtonState.semiColon = false;
      }
      if (key == GLFW_KEY_APOSTROPHE && action == GLFW_PRESS) {
        mButtonState.apostrophe = true;
      } else if (key == GLFW_KEY_APOSTROPHE && action == GLFW_RELEASE) {
        mButtonState.apostrophe = false;
      }
      if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        mButtonState.enter = true;
      } else if (key == GLFW_KEY_ENTER && action == GLFW_RELEASE) {
        mButtonState.enter = false;
      }
      // comma-period-forwardslash
      if (key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
        mButtonState.comma = true;
      } else if (key == GLFW_KEY_COMMA && action == GLFW_RELEASE) {
        mButtonState.comma = false;
      }
      if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
        mButtonState.period = true;
      } else if (key == GLFW_KEY_PERIOD && action == GLFW_RELEASE) {
        mButtonState.period = false;
      }
      if (key == GLFW_KEY_SLASH && action == GLFW_PRESS) {
        mButtonState.forwardSlash = true;
      } else if (key == GLFW_KEY_SLASH && action == GLFW_RELEASE) {
        mButtonState.forwardSlash = false;
      }
      // printscreen-etc
      if (key == GLFW_KEY_PRINT_SCREEN && action == GLFW_PRESS) {
        mButtonState.printScreen = true;
      } else if (key == GLFW_KEY_PRINT_SCREEN && action == GLFW_RELEASE) {
        mButtonState.printScreen = false;
      }
      if (key == GLFW_KEY_SCROLL_LOCK && action == GLFW_PRESS) {
        mButtonState.scrollLock = true;
      } else if (key == GLFW_KEY_SCROLL_LOCK && action == GLFW_RELEASE) {
        mButtonState.scrollLock = false;
      }
      if (key == GLFW_KEY_PAUSE && action == GLFW_PRESS) {
        mButtonState.pauseBreak = true;
      } else if (key == GLFW_KEY_PAUSE && action == GLFW_RELEASE) {
        mButtonState.pauseBreak = false;
      }
      if (key == GLFW_KEY_INSERT && action == GLFW_PRESS) {
        mButtonState.insert = true;
      } else if (key == GLFW_KEY_INSERT && action == GLFW_RELEASE) {
        mButtonState.insert = false;
      }
      if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        mButtonState.del = true;
      } else if (key == GLFW_KEY_DELETE && action == GLFW_RELEASE) {
        mButtonState.del = false;
      }
      if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
        mButtonState.home = true;
      } else if (key == GLFW_KEY_HOME && action == GLFW_RELEASE) {
        mButtonState.home = false;
      }
      if (key == GLFW_KEY_END && action == GLFW_PRESS) {
        mButtonState.end = true;
      } else if (key == GLFW_KEY_END && action == GLFW_RELEASE) {
        mButtonState.end = false;
      }
      if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS) {
        mButtonState.pageUp = true;
      } else if (key == GLFW_KEY_PAGE_UP && action == GLFW_RELEASE) {
        mButtonState.pageUp = false;
      }
      if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS) {
        mButtonState.pageDown = true;
      } else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_RELEASE) {
        mButtonState.pageDown = false;
      }
      // arrows
      if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        mButtonState.upArrow = true;
      } else if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
        mButtonState.upArrow = false;
      }
      if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        mButtonState.downArrow = true;
      } else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
        mButtonState.downArrow = false;
      }
      if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        mButtonState.leftArrow = true;
      } else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
        mButtonState.leftArrow = false;
      }
      if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        mButtonState.rightArrow = true;
      } else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
        mButtonState.rightArrow = false;
      }
      mNewKeyReads = true;

    });

    SetMouseToNormal();
    SetMouseReadToNormal();
    SetWindowClearColor();

    mGUI = new PlainGUI();
  }
  isEngineInit = true;
}
i32 Run() {
  if (!isEngineInit) {
    std::cout << "init first\n";
    return -4;
  }
  begin();
  while (!glfwWindowShouldClose(mWindow)) {
    update();
    render();
    glfwPollEvents();
  }
  teardown();
  glfwTerminate();  //todo check if crash in debug still exists
  return 0;
}
void Shutdown() noexcept {
  glfwSetWindowShouldClose(mWindow, 1);
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

  SetWindowClearColor();
}
// End Init, Run, Shutdown, Reset


// Camera
i32 AddCamera(i32 w, i32 h) {
  mCameras.emplace_back(w, h);
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
      cam.RenderProjection = RenderProjection::PERSPECTIVE;
      cam.updateProjectionMatrix();
      return;
    }
  }
  throw("cam id doesn't exist or is invalid");
}
void SetCamFOV(i32 camId, f32 new_fov) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      if (cam.RenderProjection != RenderProjection::PERSPECTIVE)
        throw("changing FOV on wrong render projection");
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
  throw("cam id doesn't exist or is invalid");}
glm::mat4 GetProjectionMatrix(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      glm::mat4 projection = glm::mat4(1);
      switch (cam.RenderProjection) {
      case RenderProjection::PERSPECTIVE:
      {
        f32 aspectRatio = static_cast<f32>(cam.Width) / static_cast<f32>(cam.Height);
        projection = glm::perspective(glm::radians(cam.FOV), aspectRatio, 0.0167f, cam.MaxRenderDistance);
      }
      break;
      case RenderProjection::ORTHO:
        projection = glm::ortho(
          0.f,
          static_cast<f32>(cam.Width),
          0.f,
          static_cast<f32>(cam.Height),
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
glm::mat4 GetOrthoMatrix(i32 camId) {
  for (auto& cam : mCameras) {
    if (cam.GetUID() == camId) {
      glm::mat4 ortho = glm::mat4(1);
      ortho = glm::ortho(
        0.f,
        static_cast<f32>(cam.Width),
        0.f,
        static_cast<f32>(cam.Height),
        0.0167f,
        cam.MaxRenderDistance
      );
      return ortho;
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


// Skybox
void SetSkybox(const std::shared_ptr<Skybox>& skybox) noexcept {
  mSkybox = skybox;
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
    mLitShader->use();
    mLitShader->setInt("isDirectionalLightOn", 1);
    mLitShader->setVec3("directionalLight.Direction", mDirectionalLight->Direction);
    mLitShader->setVec3("directionalLight.Ambient", mDirectionalLight->Ambient);
    mLitShader->setVec3("directionalLight.Diffuse", mDirectionalLight->Diffuse);
    mLitShader->setVec3("directionalLight.Specular", mDirectionalLight->Specular);
  }
}
void RemoveDirectionalLight() {
  assert(mLitShader);
  mLitShader->use();
  mLitShader->setInt("isDirectionalLightOn", 0);
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
    mLitShader->use();
    mLitShader->setInt("NUM_POINT_LIGHTS", static_cast<int>(after_size));

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
      mLitShader->use();
      mLitShader->setVec3(position.c_str(), pl.Position);
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
    mLitShader->use();
    mLitShader->setInt("NUM_SPOT_LIGHTS", static_cast<int>(after_size));

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
// End Spot Light


// Sound Effects
i32 AddSoundEffect(const char* path) {
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
void SetMouseToHidden() noexcept {
  //  If you only wish the cursor to become hidden when it is over a window but still want it to behave normally, set the cursor mode to GLFW_CURSOR_HIDDEN.
  if (glfwGetInputMode(mWindow, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
    return;  //already hidden, do nothing
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void SetMouseToDisabled() noexcept {
  //If you wish to implement mouse motion based camera controls or other input schemes that require unlimited mouse movement, set the cursor mode to GLFW_CURSOR_DISABLED.
  if (glfwGetInputMode(mWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    return;  //already disabled, do nothing
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void SetMouseToNormal() noexcept {
  //To exit out of either of these special modes, restore the GLFW_CURSOR_NORMAL cursor mode.
  if (glfwGetInputMode(mWindow, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
    return;  //already normal, do nothing
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void SetMouseReadToFPP() noexcept {
  if (mMouseReporting == MouseReporting::PERSPECTIVE)
    return;  // already in perspective handling

  mSwitchedToFPP = true;
  ::glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, f64 x, f64 y) {
    f64 xOffset = 0, yOffset = 0;
    static f64 lastX, lastY;
    if (mSwitchedToFPP) {
      mMousePosition.xOffset = 0;
      mMousePosition.yOffset = 0;
      lastX = x;
      lastY = y;
      mSwitchedToFPP = false;
    }
    xOffset = x - lastX;
    yOffset = lastY - y;
    lastX = x;
    lastY = y;
    xOffset *= mFPPMouseSensitivity;
    yOffset *= mFPPMouseSensitivity;
    mMousePosition.xOffset = xOffset;
    mMousePosition.yOffset = yOffset;
    // handle mouse pos
    for (auto& oMH : onMouseHandling) { oMH.second(mMousePosition); }
    if (glfwGetInputMode(mWindow, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
      mMousePosition.xOffset = 0;
      mMousePosition.yOffset = 0;
    }
  });

  mMouseReporting = MouseReporting::PERSPECTIVE;
}
void SetMouseFPPSensitivity(f32 sensitivity) noexcept {
  mFPPMouseSensitivity = sensitivity;
}
f32 GetMouseFPPSensitivity() noexcept {
  return mFPPMouseSensitivity;
}

void SetMouseReadToNormal() noexcept {
  if (mMouseReporting == MouseReporting::STANDARD)  // already standard
    return;
  ::glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, f64 xpos, f64 ypos) {
    mMousePosition.xOffset = xpos;
    mMousePosition.yOffset = ypos;
    if (mGUI) {
      mGUI->UpdateMouseLoc(vec2(mMousePosition.xOffset, mMousePosition.yOffset));
      //mGUI->UpdateUniforms(vec2(0), vec2(mMousePosition.xOffset, mMousePosition.yOffset), 0.f);
    }
    //std::cout << "mouse pos: " << mMousePosition.xOffset << " " << mMousePosition.yOffset << '\n';
    // handle mouse position
    for (auto& oMH : onMouseHandling) { oMH.second(mMousePosition); }
  });
  mMouseReporting = MouseReporting::STANDARD;
}
// End Mouse


void AddButton(vec2 pos, vec2 scale, vec3 color, float alpha) {
  mGUI->AddButton(pos, scale.x, scale.y, color, alpha);
}
void AddButton(vec2 pos, vec2 scale, float alpha, const char* texture_path) {
  mGUI->AddButton(pos, scale.x, scale.y, alpha, texture_path);

}
void SetGUIVisibility(const bool value) {
  if (!mGUI)
    return;
  if (value)
    mGUI->ShowInterface();
  else
    mGUI->HideInterface();
}

// Window
void SetWindowClearColor(glm::vec3 color) {
  OGLGraphics::SetViewportClearColor(color);
}
i32 GetWindowWidth() noexcept {
  i32 width, height;
  glfwGetWindowSize(mWindow, &width, &height);
  return width;
}
i32 GetWindowHeight() noexcept {
  i32 width, height;
  glfwGetWindowSize(mWindow, &width, &height);
  return height;
}
void SetWindowTitle(const char* name) noexcept {
  glfwSetWindowTitle(mWindow, name);
}
void SetReshapeCallback() noexcept {

}
// End Window


// Interface
//void AddInterfaceSlider(vec2 bot_left_loc, f32 low_val, f32 high_val, f32 bar_thickness, f32 bar_length, bool horizontal,
//  void(*returnvaluehandler)(f32 result)) {
//  if (!mGUI)
//    mGUI = new PlainGUI();
//
//  Slider tmp_slider{};
//  tmp_slider.bot_left_loc = bot_left_loc;
//  tmp_slider.low_val = low_val;
//  tmp_slider.high_val = high_val;
//  tmp_slider.bar_thickness = bar_thickness;
//  tmp_slider.bar_length = bar_length;
//  tmp_slider.horizontal = horizontal;
//  mGUI->AddSlider(tmp_slider, returnvaluehandler);
//}
// End Interface


void SetTimedOutKeyHandlingLength(const f32& newtime) {
  mNoSpamWaitLength = newtime;
}

// Loop Controls
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
