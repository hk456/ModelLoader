#include "camera.h"

namespace nelems
{
	Camera::Camera(const glm::vec3& position, float fov, float aspect, float near, float far) :
		mPosition(position), mOrientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
	{
		mFov = fov;
		mAspect = aspect;
		mNear = near;
		mFar = far;
	}

	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::inverse(glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(mOrientation));
	}

	void Camera::Pan(const glm::vec2& delta)
	{
		glm::vec3 right = GetRightDirection();
		glm::vec3 up = GetUpDirection();

		mPosition += right * delta.x;
		mPosition += up * delta.y;
	}

	void Camera::Rotate(const glm::vec2& delta)
	{
		// 1. Create tiny quaternions for the horizontal and vertical range
		glm::quat yaw = glm::angleAxis(delta.x, up);

		glm::quat pitch = glm::angleAxis(delta.y, GetRightDirection());

		// 2. Combine them 
		mOrientation = yaw * pitch * mOrientation;

		// 3. Keep it stable
		mOrientation = glm::normalize(mOrientation);
	}

	void Camera::Zoom(float delta)
	{
		mPosition += GetForwardDirection() * delta;
	}

	void Camera::update(nshaders::Shader* shader)
	{
		shader->setMat4("view", GetViewMatrix());
		shader->setMat4("projection", GetProjectionMatrix());
	}

	glm::vec3 Camera::GetUpDirection() const
	{
		return mOrientation * up;
	}

	glm::vec3 Camera::GetRightDirection() const
	{
		return mOrientation * right;
	}

	glm::vec3 Camera::GetForwardDirection() const
	{
		return mOrientation * forward;
	}
}