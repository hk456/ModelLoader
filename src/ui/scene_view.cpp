#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../pch.h"
#include "scene_view.h"

namespace nui
{
	SceneView::SceneView() :
		mCamera(nullptr), mShader(nullptr), mFramebuffer(nullptr),
		mSize(400, 300), mRenderer(nullptr)
	{
		mFramebuffer = std::make_unique<nrender::OpenGL_Framebuffer>();
		mFramebuffer->create_buffers(1300, 900);
		mShader = std::make_unique<nshaders::Shader>("shaders/vs_temp.shader", "shaders/fs_temp.shader");
		mRenderer = std::make_unique<Renderer>();
		mCamera = std::make_unique<nelems::Camera>(glm::vec3(0.0f, 0.0f, 5.0f), 45.0f, (float)mSize.x/(float)mSize.y, 0.1f, 100.0f);
	}

	void SceneView::resize(uint32_t width, uint32_t height)
	{
		mSize.x = width;
		mSize.y = height;

		mFramebuffer->create_buffers(width, height);
	}

    void SceneView::reset_view()
    {
		mCamera->reset();
    }

    void SceneView::render()
    {
        // ==========================================
        // STEP 1: IMGUI WINDOW MANAGEMENT (Calculate Sizes First)
        // ==========================================
        ImGui::SetNextWindowSizeConstraints(ImVec2(400, 300), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::Begin("Scene");

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        // Only resize if the window size has actually changed
        if (viewportPanelSize.x != mSize.x || viewportPanelSize.y != mSize.y) {
            mSize = { viewportPanelSize.x, viewportPanelSize.y };
            this->resize((int32_t)mSize.x, (int32_t)mSize.y);
        }

        // ==========================================
        // STEP 2: BIND AND CLEAR THE FRAMEBUFFER
        // ==========================================
        mFramebuffer->bind(); // BIND FIRST so glViewport and glClear know where to go!

        glViewport(0, 0, (GLsizei)mSize.x, (GLsizei)mSize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark charcoal background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ==========================================
        // STEP 3: PREPARE SHADERS & MATRICES (Before Drawing!)
        // ==========================================
        mShader->use();

        // 3a. Send Camera Matrices (View & Projection)
        mCamera->setAspect(mSize.x / mSize.y);
        mCamera->update(mShader.get());

        // 3b. Send Model Matrix (Identity / Positioning)
        glm::mat4 model = glm::mat4(1.0f);
        // If the model is too big/small or needs to be pushed up, modify it here:
        model = glm::rotate(glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        mShader->setMat4("model", model);

        // ==========================================
        // STEP 4: DRAW THE GEOMETRY
        // ==========================================
        mRenderer->Draw(mShader.get());

        mFramebuffer->unbind(); // Safely unbind back to default buffer

        // ==========================================
        // STEP 5: PASS THE FINAL TEXTURE TO IMGUI
        // ==========================================
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

    void SceneView::on_mouse_move(double x, double y, nelems::EInputButton button)
    {
        mCamera->on_mouse_move(x, y, button);
    }
}