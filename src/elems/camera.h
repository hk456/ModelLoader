#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "element.h"
#include "input.h"
#include "../shader/shader_util.h"
#include <glm/glm.hpp>

namespace nelems
{
	class Camera : public Element
	{
	public:
		Camera(const glm::vec3& position, float fov, float aspect, float near, float far);

		// -- The Core outputs --
		inline glm::mat4 GetViewMatrix();
		inline glm::mat4 GetProjectionMatrix() const { return mProjectionMatrix; }

		// -- The Inputs (Higher logic) --
		void Pan(const glm::vec2& delta);
		void Rotate(const glm::vec2& delta);
		void Zoom(float delta);
		void reset();
		void on_mouse_wheel(double delta);
		void on_mouse_move(double x, double y, EInputButton button);

		// -- State updaters --
		void setAspect(float aspect) { mProjectionMatrix = glm::perspective(mFov, aspect, mNear, mFar); }
		void setDistance(float offset);

		// Inherited from Element
		void update(nshaders::Shader* shader) override;

	private:

		// -- The State --
		glm::vec3 mPosition;
		glm::quat mOrientation;
		glm::vec3 mFocus = { 0.0f, 0.0f, 0.0f };
		glm::vec2 mCurrentMousePos = { 0.0f, 0.0f };
		bool mFirstMouse = true;
		EInputButton mLastButton = EInputButton::None; 

		// -- Cached Matrix --
		bool mViewDirty = true;
		glm::mat4 mViewMatrix;
		glm::mat4 mProjectionMatrix;

		// -- Lens Settings --
		float mDistance;
		float mPitch = 0.0f;
		float mYaw = 0.0f;
		float mFov;
		float mAspect;
		float mNear;
		float mFar;
		float mSensitivity = 0.001f;
		float mPanSpeed = 0.001f;

		// -- Helper functions for math --	
		void update_view_matrix();
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		// -- pre-defined variables --
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	};
}
