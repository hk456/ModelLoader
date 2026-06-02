#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../pch.h"
#include "scene_view.h"

namespace nui
{
	SceneView::SceneView() :
		mCamera(nullptr), mShader(nullptr), mFramebuffer(nullptr),
		mSize(1300, 900)
	{
		mFramebuffer = std::make_unique<nrender::OpenGL_Framebuffer>();
		mFramebuffer->create_buffers(1300, 900);
		mShader = std::make_unique<nshaders::Shader>("shaders/vsPbr.shader", "shaders/fsPbr.shader");
		mCamera = std::make_unique<nelems::Camera>(glm::vec3(0.0f, 0.0f, 3.0f), 45.0f, 1.3f, 0.1f, 100.0f);
	}

	void SceneView::resize(int32_t width, int32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		
	}

	void SceneView::render()
	{
		// activate the shader
		mShader->use();

		// set all the lighting uniforms

		// activate the framebuffer
		mFramebuffer->bind();

		if (mModel)
		{
			mModel->update(mShader.get());
		}

		mFramebuffer->unbind();

		ImGui::Begin("Scene");
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mSize = { viewportPanelSize.x, viewportPanelSize.y };

		mCamera->setAspect(mSize.x / mSize.y);
		mCamera->update(mShader.get());

		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFramebuffer->get_texture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

	}

	void SceneView::load_model(const std::string& filepath)
	{
	}
}