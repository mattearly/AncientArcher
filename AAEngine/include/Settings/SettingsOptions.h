#pragma once
#include "../Renderer/RenderFrameworks.h"
namespace AA {
enum class WindowingType { FULLSCREEN, WINDOWED, BORDERLESS };
struct SettingsOptions
{
	RenderingFramework renderer = RenderingFramework::OPENGL;
	WindowingType windowType = WindowingType::WINDOWED;
	int currWindowWidth = 0;
	int currWindowHeight = 0;
	int lastWindowWidth = 0;
	int lastWindowHeight = 0;
};

}