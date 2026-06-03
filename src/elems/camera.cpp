#include "camera.h"

namespace nelems
{
	Camera::Camera(const glm::vec3& position, float fov, float aspect, float near, float far) :
		mFov(fov), mAspect(aspect), mNear(near), mFar(far)
	{
		mFocus = glm::vec3(0.0f);
		mDistance = glm::distance(position, mFocus);

		// Initialize orientation based on starting position
		// or just start with default angles
		mYaw = 0.0f;
		mPitch = 0.0f;

		update_view_matrix();
		mProjectionMatrix = glm::perspective(mFov, mAspect, mNear, mFar);
	}

	inline glm::mat4 Camera::GetViewMatrix()
	{
		if (mViewDirty) {
			update_view_matrix();
			mViewDirty = false;
		}
		return mViewMatrix;
	}

	glm::mat4 Camera::update_view_matrix()
	{
		// Calculate the direction from Pitch and Yaw
		mOrientation = glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));

		// update position relative to focus
		mPosition = mFocus - GetForwardDirection() * mDistance;
		
		mViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(mOrientation));
	}

	void Camera::Pan(const glm::vec2& delta)
	{
		mFocus += -GetRightDirection() * delta.x * mDistance;
		mFocus += GetUpDirection() * delta.y * mDistance;
		mViewDirty = true;
	}

	void Camera::Rotate(const glm::vec2& delta)
	{
		mYaw += delta.x;
		mPitch += delta.y;
		mViewDirty = true;
	}

	void Camera::Zoom(float delta)
	{
		mPosition += GetForwardDirection() * delta;
	}

	void Camera::reset()
	{
		mFocus = { 0.0f, 0.0f, 0.0f };
		update_view_matrix();
	}

	void Camera::update(nshaders::Shader* shader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		shader->setMat4("model", model);
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