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
		// Setup ImGui constraints and window FIRST so we can get sizes
		ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(FLT_MAX, FLT_MAX));
		ImGui::Begin("Scene");

		// Get the current size of the ImGui window content area
		ImVec2 viewPortPanelSize = ImGui::GetContentRegionAvail();

		// Handle scaling/resizing dynamically
		if (viewPortPanelSize.x != mSize.x || viewPortPanelSize.y != mSize.y) {
			mSize = { viewPortPanelSize.x, viewPortPanelSize.y };
			this->resize((uint32_t)mSize.x, (uint32_t)mSize.y);
		}

		// Now start drawing
		mFramebuffer->bind();

		// resize the window to the size of the scene window
		glViewport(0, 0, (GLsizei)mSize.x, (GLsizei)mSize.y);

		// clear the custom framebuffer's color and depth buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate the shader
		mShader->use();

		// update and upload camera matrices
		mCamera->setAspect(mSize.x / mSize.y);
		mCamera->update(mShader.get());

		// draw the model if it exists
		if (mModel)
		{
			mModel->update(mShader.get());
			mModel->Draw(mShader.get());
		}

		// unbind the framebuffer
		mFramebuffer->unbind();

		// Add the rendered texture to the ImGui viewport window
		uint64_t textureID = mFramebuffer->get_texture();

		// ADD THIS TEMPORARY PRINT:
		static int frameCount = 0;
		if (frameCount++ % 60 == 0) { // Prints once every 60 frames so it doesn't spam too fast
			std::cout << "DEBUG: Texture ID = " << textureID
				<< " | Render Size = " << mSize.x << "x" << mSize.y << "\n";
		}

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