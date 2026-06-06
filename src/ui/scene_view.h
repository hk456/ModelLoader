#pragma once

#include "../render/opengl_buffer_manager.h"
#include "../elems/camera.h"
#include "../elems/model.h"
#include "../shader/shader_util.h"
#include "renderer.h"

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

		void resize(uint32_t width, uint32_t height);

		void render();

		void load_model(const std::string& filepath);

		nelems::Model* get_model();

		void set_model(std::unique_ptr<nelems::Model> model)
		{
			mModel = std::move(model);
		}

		void on_mouse_wheel(double delta);

	private:
		std::unique_ptr<nelems::Camera> mCamera;
		std::unique_ptr<nrender::OpenGL_Framebuffer> mFramebuffer;
		std::unique_ptr<nelems::Model> mModel;
		std::unique_ptr<nshaders::Shader> mShader;
		std::unique_ptr<Renderer> mRenderer;
		glm::vec2 mSize;
	};
}
