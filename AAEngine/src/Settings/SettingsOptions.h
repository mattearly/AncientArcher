#pragma once
#include "../WindowingType.h"
#include "../RenderingFramework.h"
namespace AA {
struct SettingsOptions
{
  RenderingFramework renderer = RenderingFramework::OPENGL;
  int RendererVersionMajor = -1;
  int RendererVersionMinor = -1;
  bool MSAA = true;
  int msaa_samples = 4;
  WindowingType windowType = WindowingType::WINDOWED;
  int default_window_width = 1280;
  int default_window_height = 720;
  bool vsync_enabled = false;
  bool drag_and_drop_files_support = true;
};

}