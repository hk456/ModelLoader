#include "pch.h"
#include "application.h"

int main()
{
	auto app = std::make_unique<Application>("MeshBox");
	app->loop();

	return 0;
}