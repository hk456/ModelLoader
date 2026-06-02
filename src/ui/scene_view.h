#pragma once

#include "../render/opengl_buffer_manager.h"
#include "../elems/camera.h"
#include "../elems/model.h"
#include "../shader/shader_util.h"

namespace nui
{
	class SceneView
	{
	public:
		SceneView();

		~SceneView()
		{
			mShader->unload();
		}

		void resize(int32_t width, int32_t height);

		void render();

		void load_model(const std::string& filepath);

		void set_model(std::unique_ptr<nelems::Model> model)
		{
			mModel = std::move(model);
		}

	private:
		std::unique_ptr<nelems::Camera> mCamera;
		std::unique_ptr<nrender::OpenGL_Framebuffer> mFramebuffer;
		std::unique_ptr<nelems::Model> mModel;
		std::unique_ptr<nshaders::Shader> mShader;
		glm::vec2 mSize;
	};
}
