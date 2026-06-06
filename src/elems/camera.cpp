#include "camera.h"

namespace nelems
{
	Camera::Camera(const glm::vec3& position, float fov, float aspect, float near, float far) :
		mFov(fov), mAspect(aspect), mNear(near), mFar(far)
	{
		mFocus = glm::vec3(0.0f);
		mDistance = glm::distance(position, mFocus);

		mYaw = 0.0f;
		mPitch = 0.0f;

		update_view_matrix();
		mProjectionMatrix = glm::perspective(glm::radians(mFov), mAspect, mNear, mFar);
	}

	inline glm::mat4 Camera::GetViewMatrix()
	{
		if (mViewDirty) {
			update_view_matrix();
			mViewDirty = false;
		}
		return mViewMatrix;
	}

	void Camera::update_view_matrix()
	{
		// FIX: Convert Pitch and Yaw from degrees to RADIANS before building the quaternion
		glm::quat qPitch = glm::angleAxis(glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat qYaw = glm::angleAxis(glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

		mOrientation = qYaw * qPitch;

		// Update position relative to focus point
		mPosition = mFocus - GetForwardDirection() * mDistance;

		mViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(mOrientation));
	}

	void Camera::Pan(const glm::vec2& delta)
	{
		// Moving your focus point scales beautifully based on how far away you are zooming
		mFocus += -GetRightDirection() * delta.x * mPanSpeed * (mDistance * 0.1f);
		mFocus += GetUpDirection() * delta.y * mPanSpeed * (mDistance * 0.1f);
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
		mDistance -= delta;
		if (mDistance < 0.1f) mDistance = 0.1f; // Prevent crossing past the focus target
		mViewDirty = true;
	}

	void Camera::on_mouse_wheel(double delta)
	{
		// Scroll up zooms in (reduces distance), scroll down zooms out
		Zoom(static_cast<float>(delta) * 0.5f);
	}

	void Camera::reset()
	{
		mFocus = { 0.0f, 0.0f, 0.0f };
		mYaw = 0.0f;
		mPitch = 0.0f;
		mViewDirty = true;
	}

	void Camera::on_mouse_move(double x, double y, EInputButton button)
	{
		if (button == EInputButton::None)
		{
			mFirstMouse = true;
			mLastButton = button;
			return;
		}

		if (button != mLastButton)
		{
			mFirstMouse = true;
		}
		mLastButton = button;

		if (mFirstMouse)
		{
			mCurrentMousePos = { x, y };
			mFirstMouse = false;
			return;
		}

		glm::vec2 offset = glm::vec2(x, y) - mCurrentMousePos;
		mCurrentMousePos = { x, y };

		if (button == EInputButton::Right)
		{
			// ROTATION (Look Around)
			mYaw += offset.x * mSensitivity;
			mPitch += offset.y * mSensitivity;

			// Clamp pitch to avoid flipping upside down
			if (mPitch > 89.0f)  mPitch = 89.0f;
			if (mPitch < -89.0f) mPitch = -89.0f;

			mViewDirty = true;
		}
		else if (button == EInputButton::Middle)
		{
			// FIX: Use your dedicated Pan method! It passes both x and y offsets
			// and correctly modifies the focus point instead of getting wiped out.
			Pan(offset);
		}
	}

	void Camera::setDistance(float offset)
	{
		mDistance = offset;
		mViewDirty = true;
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