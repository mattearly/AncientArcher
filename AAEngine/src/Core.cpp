#include "../include/AncientArcher/Types.h"
#include "../include/AncientArcher/Utility/Files.h"
#include "../include/AncientArcher/Controls/KeyboardInput.h"
#include "../include/AncientArcher/Controls/MouseInput.h"
#include "../include/AncientArcher/Controls/ScrollInput.h"
#include "GUI/PlainGUI.h"
#include "GUI/imGUI.h"
#include "Scene/Lights.h"
#include "Scene/Camera.h"
#include "Scene/Prop.h"
#include "Renderer/OpenGL/OGLGraphics.h"
#include "Renderer/ModelLoader.h"
#include "Sound/SoundDevice.h"
#include "Sound/Speaker.h"
#include "Sound/SoundEffect.h"
#include "Sound/LongSound.h"
#include "Settings/Settings.h"
#include "MouseReporting.h"
#include "Core.h"
#include "Utility/QueryShader.h"
#include "Scene/Skybox.h"
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace AA {
namespace Core {
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
void SetMouseReadToNormal() noexcept {
  if (mMouseReporting == MouseReporting::STANDARD)  // already standard
    return;
  ::glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, f64 xpos, f64 ypos) {
    mMousePosition.xOffset = xpos;
    mMousePosition.yOffset = ypos;
    if (mGUI) {
      mGUI->UpdateMouseLoc(vec2(mMousePosition.xOffset, mMousePosition.yOffset));
    }
    for (auto& oMH : onMouseHandling) { oMH.second(mMousePosition); }
  });
  mMouseReporting = MouseReporting::STANDARD;
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
void SetWindowClearColor(glm::vec3 color) noexcept {
  OGLGraphics::SetViewportClearColor(color);
}
void SetWindowTitle(const char* name) noexcept {
  glfwSetWindowTitle(mWindow, name);
}
void SetWindowFullscreen(const bool status) noexcept {
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  if (status == true) {
    glfwSetWindowMonitor(mWindow, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
  } else {
    glfwSetWindowMonitor(
      mWindow,
      nullptr,
      mode->width / 2.f - Settings::Get()->GetOptions().default_window_width / 2.f,  // middle of screen then up to the left half the window size
      mode->height / 2.f - Settings::Get()->GetOptions().default_window_height / 2.f,  // should center the screen
      Settings::Get()->GetOptions().default_window_width,
      Settings::Get()->GetOptions().default_window_height,
      GLFW_DONT_CARE);
  }
}
}
void SetMouseToHidden() noexcept {
  //  If you only wish the cursor to become hidden when it is over a window but still want it to behave normally, set the cursor mode to GLFW_CURSOR_HIDDEN.
  if (glfwGetInputMode(mWindow, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN)
    return;  //already hidden, do nothing
  glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void SetMouseToDisabled() noexcept {
  // If you wish to implement mouse motion based camera controls or other input schemes that require unlimited mouse movement, set the cursor mode to GLFW_CURSOR_DISABLED.
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
void Init() {
  if (isEngineInit)
    throw("already initialized");
  isEngineInit = true;

  SoundDevice::Init();

  // set an error calback in case of failure we at least know
  glfwSetErrorCallback([](i32 e, const char* msg) {
    if (e != 65543)
      throw("glfw callback error");
  });

  glfwInit();

  auto local_options = Settings::Get()->GetOptions();

  if (local_options.windowType == WindowingType::MAXIMIZED) {
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
  }

  if (local_options.MSAA == true) {
    glfwWindowHint(GLFW_SAMPLES, local_options.msaa_samples);
  }

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
    mWindow = glfwCreateWindow(local_options.default_window_width, local_options.default_window_height, "AncientArcher Default Window Title", nullptr, nullptr);
    if (!mWindow) {
      try_versions.pop_back();
    } else  // save results to settings
    {
      local_options.RendererVersionMajor = try_versions.back().major;
      local_options.RendererVersionMinor = try_versions.back().minor;
    }
  }

  if (!mWindow)
    throw("unable to init OpenGL 4.3+");

  glfwSetWindowSizeLimits(mWindow, MINSCREENWIDTH, MINSCREENHEIGHT, MAXSCREENWIDTH, MAXSCREENHEIGHT);

  glfwMakeContextCurrent(mWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))  // tie window context to glad's opengl funcs
  {
    throw("Unable to context to OpenGL");
  }
  OGLGraphics::SetMSAA(local_options.MSAA);
  OGLGraphics::SetBlend(true);
  if (local_options.vsync_enabled) {
    glfwSwapInterval(1);
  }

  // set all our options to what we set (mainly the major and minor version will be updated)
  Settings::Get()->SetOptions(local_options);

  mGUI = new PlainGUI();

  SetMouseToNormal();

  SetMouseReadToNormal();

  //callbacks
  ::glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, i32 w, i32 h) {
    OGLGraphics::SetViewportSize(0, 0, w, h);
    for (auto& cam : mCameras) {
      cam.updateProjectionMatrix();
#if _DEBUG
      std::cout << "projection updated for cam " << cam.GetUID() << '\n';
#endif
    }
    if (mDiffShader) {
      mDiffShader->Use();
      mDiffShader->SetMat4("u_projection_matrix", mCameras.front().mProjectionMatrix);
    }
    if (mLitShader) {
      mLitShader->Use();
      mLitShader->SetMat4("u_projection_matrix", mCameras.front().mProjectionMatrix);
    }
    if (mSkybox && !mCameras.empty()) {
      mSkybox->SetProjectionMatrix(mCameras.front().mProjectionMatrix);
    }
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

  ::glfwSetDropCallback(mWindow, [](GLFWwindow* w, int count, const char** paths) {
    if (!Settings::Get()->GetOptions().drag_and_drop_files_support)
      return;

    int i;
    for (i = 0; i < count; i++) {
#ifdef _DEBUG
      std::cout << "file dropped onto screen: " << paths[i] << '\n';
#endif

      //std::string proc_ = paths[i];


      //std::size_t the_last_slash = proc_.find_last_of("/\\") + 1;
      //std::size_t the_last_dot = proc_.find_last_of(".");

      ////std::string path_to = proc_.substr(0, the_last_slash);  // path to filename's dir

      //std::string file_extension = proc_.substr(
      //  static_cast<std::basic_string<char,
      //  std::char_traits<char>,
      //  std::allocator<char>>::size_type>(the_last_dot) + 1);  // get the file extension (type of file)

      //if (file_extension == "fbx" || file_extension == "FBX") {
      //  if (mCameras.empty()) {
      //    AddProp(paths[i], 0, SHADERTYPE::DIFF);
      //    mProps.back().translation.z = -40;
      //    //mProps.back().translation.x = -1;
      //  } else {
      //    AddProp(paths[i], mCameras.front().GetUID(), SHADERTYPE::LIT);
      //  }
      //}
  }
});


}
i32 Run() {
  if (!isEngineInit) {
#if _DEBUG
    std::cout << "Attempted Run but InitEngine has not been invoked.\n";
#endif
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
void Shutdown() {
  glfwSetWindowShouldClose(mWindow, 1);
}
}

bool isEngineInit = false;
GLFWwindow* mWindow = nullptr;
const f32 mDefaultFPPMouseSensitivity = 0.1f;
f32 mFPPMouseSensitivity = mDefaultFPPMouseSensitivity;  ///< mouse sensitivity while in first person perspective
bool mSwitchedToFPP = false;


MouseReporting mMouseReporting = MouseReporting::UNSET;
PlainGUI* mGUI = NULL;
imGUI* mimGUI = NULL;


const char* vert_path = "..\\AAEngine\\GLSL_src\\vert_3D.glsl";
const char* frag_lit_path = "..\\AAEngine\\GLSL_src\\frag_lit.glsl";
const char* frag_diff_path = "..\\AAEngine\\GLSL_src\\frag_diff.glsl";

OGLShader* mDiffShader = nullptr;
OGLShader* mLitShader = nullptr;
void setupLitShader() {
  if (!mLitShader) {
    mLitShader = new OGLShader(Files::ReadToString(vert_path).c_str(), Files::ReadToString(frag_lit_path).c_str());
#ifdef _DEBUG
    QueryInputAttribs(mLitShader->GetID());
    QueryUniforms(mLitShader->GetID());
#endif
    if (mCameras.empty())
      return;
    mLitShader->Use();
    mLitShader->SetMat4("u_projection_matrix", mCameras.front().mProjectionMatrix);
  }
}
void setupDiffShader() {
  if (!mDiffShader) {
    mDiffShader = new OGLShader(Files::ReadToString(vert_path).c_str(), Files::ReadToString(frag_diff_path).c_str());
#ifdef _DEBUG
    QueryInputAttribs(mDiffShader->GetID());
    QueryUniforms(mDiffShader->GetID());
#endif
    if (mCameras.empty())
      return;
    mDiffShader->Use();
    mDiffShader->SetMat4("u_projection_matrix", mCameras.front().mProjectionMatrix);
  }
}


// single directional light
DirectionalLight* mDirectionalLight = nullptr;  ///< directional light for lit shader(s)
                                      // point light system
i32 mNumPointLightsInUse = 0;
const i32 MAXSPOTLIGHTS = 25;  // also change on shader if changed here
std::vector<SpotLight> mSpotLights;  ///< array of current spot lights
                                     // spot light system
i32 mNumSpotLightsInUse = 0;
const i32 MAXPOINTLIGHTS = 50;  // also change on shader if changed here
std::vector<PointLight> mPointLights;  ///< array of current point lights

std::vector<Camera> mCameras;  ///< array of available cameras
void updateProjectionFromCam(OGLShader* shader_to_update, const Camera& from_cam) {
  if (!shader_to_update)
    return;
  shader_to_update->Use();
  shader_to_update->SetMat4("u_projection_matrix", from_cam.mProjectionMatrix);
}

std::vector<Prop> mProps;  ///< array of available inanimate props

std::shared_ptr<Skybox> mSkybox;  ///< the main skybox
                                  // sound systems
std::vector<Speaker*> mSpeakers;  ///< array of places to play sound effects from
std::vector<SoundEffect*> mSoundEffects;  ///< array of ready speaker id to sound effects
LongSound* mMusic = nullptr;  ///< background music
                    // input, timing, and update
f32 mNonSpammableKeysTimeout = 0;  ///< keeps track of how long the keys have timed out
f32 mSlowUpdateTimeout = 0.f;  ///< keeps track of how how long the slow update has been timed out
f32 mNoSpamWaitLength = 0.2359f;  ///< how long the non-spammable keys are to time out for at least
f32 mSlowUpdateWaitLength = 0.1259f;  ///< ms length the slow update times out for at least
std::unordered_map<u32, std::function<void()> > onBegin;  ///< list of functions to run once when runMainAncientArcher is called
std::unordered_map<u32, std::function<void(f32)> > onDeltaUpdate;  ///< list of functions that rely on deltatime in the main AncientArcher
std::unordered_map<u32, std::function<void()> > onUpdate;  ///< list of functions that run every frame in the main AncientArcher
std::unordered_map<u32, std::function<void()> > onSlowUpdate;  ///< list of functions to run every only every mSlowUpdateWaitLength in the main AncientArcher
std::unordered_map<u32, std::function<bool(KeyboardInput&)> > onTimeoutKeyHandling;  ///< list of functions to handle key presses that time out for mNoSpamWaitLength after press
std::unordered_map<u32, std::function<void(ScrollInput&)> > onScrollHandling;  ///< list of functions to handle mouse wheel scroll every frame in the main AncientArcher
std::unordered_map<u32, std::function<void(KeyboardInput&)> > onKeyHandling;  ///< list of functions to handle keypresses every frame in the main AncientArcher
std::unordered_map<u32, std::function<void(MouseInput&)> > onMouseHandling;  ///< list of functions to handle mouse movement every frame in the main AncientArcher
std::unordered_map<u32, std::function<void()> > onTearDown;  ///< list of functions to run when destroying
KeyboardInput mButtonState = {};
MouseInput    mMousePosition = {};
ScrollInput   mMouseWheelScroll = {};
bool          mNewKeyReads = false;
// screen size limits
const i32     MINSCREENWIDTH = 100;
const i32     MINSCREENHEIGHT = 100;
const i32     MAXSCREENWIDTH = 7680;  //8k
const i32     MAXSCREENHEIGHT = 4320;
// core routines

void begin() {
  glfwSetWindowShouldClose(mWindow, 0);
  for (const auto& oB : onBegin) {
    oB.second();
  }
}

void update() {
  // init delta clock on first tap into update
  static float currTime;
  static float lastTime = (float)glfwGetTime();
  f32 elapsedTime;

  // update engine run delta times
  currTime = (float)glfwGetTime();
  elapsedTime = currTime - lastTime;
  lastTime = currTime;

  // process keyboard input
  mNonSpammableKeysTimeout += elapsedTime;
  if (mNewKeyReads) {
    // regular key updates
    for (auto& oKH : onKeyHandling) { oKH.second(mButtonState); }
    // non-spammable key updates
    if (mNonSpammableKeysTimeout > mNoSpamWaitLength) {
      for (auto& oTOK : onTimeoutKeyHandling) {
        if (oTOK.second(mButtonState)) {
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
    for (auto& oSU : onSlowUpdate) { oSU.second(); }
    mSlowUpdateTimeout = 0.f;
  }

  if (mMusic) {
    static f32 music_rebuffer_cd = 0;
    music_rebuffer_cd += elapsedTime;
    if (music_rebuffer_cd > .5f) {  // todo(maybe): math with file size and stuff to figure out how long this cd should actually be
      mMusic->UpdatePlayBuffer();
      music_rebuffer_cd = 0;
    }
  }

  // run through every frame update
  for (auto& oU : onUpdate) { oU.second(); }
}

void render() {
  OGLGraphics::ClearScreen();

  //if (isWindowSizeDirty) {
  //  if (mCameras.empty()) {
  //    float aspect_ratio = (float)Core::GetWindowWidth() / Core::GetWindowHeight();
  //    float ortho_height = Core::GetWindowHeight() / 2.f;
  //    float ortho_width = ortho_height * aspect_ratio;

  //    if (mLitShader) {
  //      mLitShader->setMat4("u_projection_matrix", glm::ortho(-ortho_width, ortho_width, -ortho_height, ortho_height, .1f, 2000.f));
  //      mLitShader->setMat4("u_view_matrix", glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));
  //    }
  //    if (mDiffShader) {
  //      mDiffShader->setMat4("u_projection_matrix", glm::ortho(-ortho_width, ortho_width, -ortho_height, ortho_height, .1f, 2000.f));
  //      mDiffShader->setMat4("u_view_matrix", glm::lookAt(glm::vec3(0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)));
  //    }
  //  } else {
  //    if (mLitShader) {
  //      updateProjectionFromCam(mLitShader, mCameras.front());  //todo: not this front thing from cam blindly
  //    }
  //    if (mDiffShader) {
  //      updateProjectionFromCam(mDiffShader, mCameras.front());
  //    }
  //    if (mSkybox) {
  //      mSkybox->SetProjectionMatrix(mCameras.front().mProjectionMatrix);
  //    }
  //  }
  //  isWindowSizeDirty = false;
  //}

  for (auto& p : mProps) {
    switch (p.mShaderType) {
    case SHADERTYPE::LIT:
      if (!mLitShader) break;

      mLitShader->Use();
      if (mCameras.empty())
        mLitShader->SetMat4("u_view_matrix", glm::mat4(1));
      else
        mLitShader->SetMat4("u_view_matrix", mCameras.front().mViewMatrix);
      break;
    case SHADERTYPE::DIFF:
      if (!mDiffShader) break;

      mDiffShader->Use();
      if (mCameras.empty())
        mDiffShader->SetMat4("u_view_matrix", glm::mat4(1));
      else
        mDiffShader->SetMat4("u_view_matrix", mCameras.front().mViewMatrix);
      break;
    }

    p.Draw();
  }

  //for (auto& ap : mAnimProps) {
  //  mAnimDiffShader->use();
  //  mAnimDiffShader->setMat4("view", mCameras.front().mViewMatrix);
  //  ap.Draw();
  //}

  //if (mSkybox && !mCameras.empty()) { mSkybox->render(mCameras.front()); }
  if (mSkybox && !mCameras.empty())
  {
    mSkybox->SetProjectionMatrix(mCameras.front().mProjectionMatrix);
    mSkybox->SetViewMatrix(mCameras.front().mViewMatrix);
    mSkybox->Render();
  }

  // if there is a gui
  if (mGUI) {
    mGUI->Draw();
  }

  if (mimGUI) {
    mimGUI->NewFrame();
    mimGUI->Update();
    mimGUI->Render();
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

  // delete all the meshes and textures from animated props from GPU memory
  //for (const auto& ap : mAnimProps) {
  //  ModelLoader::UnloadGameObject(ap.mMeshes);
  //}

  if (mimGUI) {
    mimGUI->Shutdown();
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



}  // end namespace AA
