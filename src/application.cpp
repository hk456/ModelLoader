#include "application.h"

Application::Application(const std::string& app_name)
{
	mWindow = std::make_unique<nwindow::GLWindow>();
	mWindow->init(1300, 900, "yeah");
}

void Application::loop()
{
	while (mWindow->is_running())
	{
		mWindow->render();
	}
}
