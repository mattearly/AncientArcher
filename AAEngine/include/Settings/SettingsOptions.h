#pragma once
#include "../Renderer/RenderFrameworks.h"
namespace AA {
enum class WindowingType { FULLSCREEN, WINDOWED, BORDERLESS };
struct SettingsOptions
{
	RenderingFramework renderer = RenderingFramework::OPENGL;
	WindowingType windowType = WindowingType::WINDOWED;
};

}