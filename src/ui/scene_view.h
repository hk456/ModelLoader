#pragma once

#include "../render/opengl_buffer_manager.h"

namespace nui
{
	class SceneView
	{
	public:
		SceneView()
		{

		}

		~SceneView()
		{

		}

		void resize(int32_t width, int32_t height);

		void render();

		void load_model(const std::string& filepath);


	private:
		glm::vec2 mSize;
	};
}
