#include "pch.h"
#include "application.h"
#include <filesystem>

int main()
{
	std::cout << "==================================\n";
	std::cout << "CURRENT WORKING DIR: " << std::filesystem::current_path() << "\n";
	std::cout << "==================================\n";

	auto app = std::make_unique<Application>("MeshBox");
	app->loop();

	return 0;
}