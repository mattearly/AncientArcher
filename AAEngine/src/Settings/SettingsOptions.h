#pragma once
namespace AA {
enum class WindowingType { FULLSCREEN, WINDOWED, BORDERLESS };
enum class RenderingFramework { OPENGL, D3D, VULKAN };
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
};

}