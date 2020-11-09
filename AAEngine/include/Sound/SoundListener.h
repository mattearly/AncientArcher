#pragma once
#include <AL/al.h>
#include <glm\ext\vector_float3.hpp>
namespace AA {
class SoundListener
{
public:
	static SoundListener* Get();
	void SetMasterGain(const float& gain);
	void SetPosition(const glm::vec3& pos);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetLocation(const glm::vec3& pos);
	void SetLocation(const float& x, const float& y, const float& z);

	void SetOrientation(const glm::vec3& at, const glm::vec3& up);
	void SetOrientation(
		const float& xat, const float& yat, const float& zat,
		const float& xup, const float& yup, const float& zup);

private:
	SoundListener();
	~SoundListener();

};

} // end namespace AA