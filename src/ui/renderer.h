#pragma once

#include "../shader/shader_util.h"
#include "../pch.h"

class Renderer
{
public:
	void Draw(nshaders::Shader* shader);
private:
	void renderCube(nshaders::Shader* shader);
};
