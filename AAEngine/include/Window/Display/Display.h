#pragma once
#include <glm/glm.hpp>
#include "../Controls/Controls.h"

struct GLFWwindow;

namespace AA
{
enum class MouseReporting { STANDARD, PERSPECTIVE };

class Display : public Controls
{
public:
	~Display();

	int GetWindowWidth() noexcept;
	int GetWindowHeight() noexcept;
	GLFWwindow* GetWindow() noexcept;
	int GetCursorMode() const noexcept;
	MouseReporting GetMouseReportingMode() const noexcept;

	void SetClearColor(glm::vec3 color = glm::vec3(.35f, .15f, .35f));

	void SetCursorToVisible() noexcept;   // to use the os pointer
	void SetCursorToHidden() noexcept;    // for hidden but still there (render your own pointer)
	void SetCursorToDisabled() noexcept;  // for first person hidden mouse type

	void SetReadMouseCurorAsFPP() noexcept;
	void SetReadMouseCurorAsStandard() noexcept;

	void SetWindowTitle(const char* name) noexcept;

	void ReshapeWindowHandler(int width, int height);
	void SetupReshapeCallback() noexcept;

protected:

	void SetupScrollWheelCallback() noexcept;

	void pullButtonStateEvents();

	void clearBackBuffer() const noexcept;
	void swapWindowBuffers() const noexcept;

//private:
	MouseReporting mMouseReporting = MouseReporting::STANDARD;

	GLFWwindow* mWindow = nullptr;

	void closeWindow() noexcept;
	void keepWindowOpen() noexcept;

	bool isTryingToClose() noexcept;
	bool isFPP() noexcept;
	Display();

private:


};
}  // end namespace AA