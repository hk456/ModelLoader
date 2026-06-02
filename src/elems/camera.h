#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "element.h"
#include "../shader/shader_util.h"
#include <glm/glm.hpp>

namespace nelems
{
	class Camera : public Element
	{
	public:
		Camera(const glm::vec3& position, float fov, float aspect, float near, float far);

		// -- The Core outputs --
		glm::mat4 GetViewMatrix() const;
		inline glm::mat4 GetProjectionMatrix() const { return mProjectionMatrix; }

		// -- The Inputs (Higher logic) --
		void Pan(const glm::vec2& delta);
		void Rotate(const glm::vec2& delta);
		void Zoom(float delta);

		// -- Setters --
		void setAspect(float aspect) { mProjectionMatrix = glm::perspective(mFov, aspect, mNear, mFar); }

		// Inherited from Element
		void update(nshaders::Shader* shader) override;

	private:

		// -- The State --
		glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 5.0f);
		glm::quat mOrientation;

		// -- Cached Matrix --
		glm::mat4 mProjectionMatrix;

		// -- Lens Settings --
		float mFov;
		float mAspect;
		float mNear;
		float mFar;

		// -- Helper functions for math --
		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		// -- pre-defined variables --
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
	};
}
