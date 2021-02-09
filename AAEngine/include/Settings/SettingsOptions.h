#pragma once
#include "../Renderer/RenderFrameworks.h"
namespace AA {
enum class WindowingType { FULLSCREEN, WINDOWED, BORDERLESS };
struct SettingsOptions
{
	RenderingFramework renderer = RenderingFramework::OPENGL;
	WindowingType windowType = WindowingType::WINDOWED;
	bool MSAA = true;
	int msaa_samples = 4;
	int default_window_width = 1280;
	int default_window_height = 720;
};

}