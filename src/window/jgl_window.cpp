#include "jgl_window.h"

namespace nwindow
{
	bool GLWindow::init(int width, int height, const std::string& title)
	{
		Width = width;
		Height = height;
		Title = title;

		return mIsRunning;	
	}

	GLWindow::~GLWindow()
	{
		mUICtx->end();

		mRenderCtx->end();
	}

	void GLWindow::render()
	{
		// Clear the view
		mRenderCtx->pre_render();

		// Initialize UI components
		mUICtx->pre_render();

		// render scene to framebuffer and add it to scene view
		mSceneView->render();

		mPropertyPanel->render(mSceneView.get());

		// Render the UI
		mUICtx->post_render();

		// Render end, swap buffers
		mRenderCtx->post_render();

		handle_input();
	}

	void GLWindow::handle_input()
	{
	}

	void GLWindow::on_key(int key, int scancode, int action, int mods)
	{
	}

	void GLWindow::on_resize(int width, int height)
	{
		Width = width;
		Height = height;

		mSceneView->resize(Width, Height);
		render();
	}

	void GLWindow::on_close()
	{
		mIsRunning = false;
	}
}