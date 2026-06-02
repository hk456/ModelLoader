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
	}

	void GLWindow::render()
	{
	}

	void GLWindow::handle_input()
	{
	}

	void GLWindow::on_key(int key, int scancode, int action, int mods)
	{
	}

	void GLWindow::on_resize(int width, int height)
	{
	}

	void GLWindow::on_close()
	{
	}
}