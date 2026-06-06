#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../pch.h"
#include "scene_view.h"

namespace nui
{
	SceneView::SceneView() :
		mCamera(nullptr), mShader(nullptr), mFramebuffer(nullptr),
		mSize(400, 300)
	{
		mFramebuffer = std::make_unique<nrender::OpenGL_Framebuffer>();
		mFramebuffer->create_buffers(1300, 900);
		mShader = std::make_unique<nshaders::Shader>("shaders/vs.shader", "shaders/fs.shader");
		mCamera = std::make_unique<nelems::Camera>(glm::vec3(0.0f, 0.0f, 3.0f), 45.0f, (float)mSize.x/(float)mSize.y, 0.1f, 100.0f);
	}

	void SceneView::resize(uint32_t width, uint32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		mFramebuffer->create_buffers(width, height);
	}

	void SceneView::render()
	{
		// activate the shader
		mShader->use();

		// activate the framebuffer
		mFramebuffer->bind();

		if (mModel)
		{
			mModel->update(mShader.get());
			mModel->Draw(mShader.get());
		}

		mFramebuffer->unbind();

		// Set a minimum size of 400x300, no maximum limit
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(FLT_MAX, FLT_MAX));

		ImGui::Begin("Scene");
	
		// get the current size of the ImGui window content area
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		// Only resize if the window size has actually changed
		if (viewportPanelSize.x != mSize.x || viewportPanelSize.y != mSize.y) {
			mSize = { viewportPanelSize.x, viewportPanelSize.y };
			this->resize((int32_t)mSize.x, (int32_t)mSize.y);
		}

		mCamera->setAspect(mSize.x / mSize.y);
		mCamera->update(mShader.get());

		// add rendered texture to ImGUI scene window
		uint64_t textureID = mFramebuffer->get_texture();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ mSize.x, mSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();

	}

	void SceneView::load_model(const std::string& filepath)
	{
		mModel = std::make_unique<nelems::Model>(filepath);
	}

	nelems::Model* SceneView::get_model()
	{
		return mModel.get();
	}

	void SceneView::on_mouse_wheel(double delta)
	{
		mCamera->on_mouse_wheel(delta);
	}
}