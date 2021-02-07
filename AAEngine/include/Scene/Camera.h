#pragma once
#include <glm/glm.hpp>
#include <memory>

namespace AA
{
enum class RenderProjection { ORTHO, PERSPECTIVE };

class Camera
{
public:
	Camera(int width, int height);


	// setters
	void SetMaxRenderDistance(float distance) noexcept;

	void SetToPerspective() noexcept;
	void SetDimensions(int width, int height) noexcept;
	void SetCurrentLocation(glm::vec3 pos);
	void SetCurrentPitch(float pitch);
	void SetCurrentYaw(float yaw);

	void ShiftCurrentLocation(const glm::vec3& offset);
	void ShiftYawAndPitch(float yawOffset, float pitchOffset);

	// experimental
	void __setToOrtho() noexcept;

	// getters
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	const glm::vec3& GetLocation() const noexcept;
	const glm::vec3* GetFront() const noexcept;
	const glm::vec3* GetRight() const noexcept;
	float GetYaw() const noexcept;
	float GetPitch() const noexcept;
	float GetRenderDistance() const noexcept;
	const int& GetID() const noexcept;

private:

	int              mWidth;
	int              mHeight;
	glm::vec3        mFront;
	glm::vec3        mRight;
	glm::vec3        mUp;
	glm::vec3        mPosition;
	float            mPerspectiveFieldOfView;
	float            mYaw;
	float            mPitch;
	float            mMaxRenderDistance;
	//bool             mProjectionChanged;
	//glm::vec4        mOrthoFieldSize;
	int              mUniqueViewportID;
	RenderProjection mRenderProjection;

	void resetViewportVars();
	void updateCameraVectors();

};
} // end namespace AA
