#pragma once

#include "vertex.h"
#include "texture.h"
#include "../pch.h"
#include "../shader/shader_util.h"
#include "element.h"

namespace nelems
{
	class Mesh : public Element
	{
	public:
		// Mesh data
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<Texture>		textures;
		unsigned int VAO;

		// Constructor
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		
		// Destructor
		~Mesh();

		// Public Draw Function
		void draw(nshaders::Shader* shader);

		// update uniforms
		void update(nshaders::Shader* shader) override;

	private:
		// Render data
		unsigned int VBO, EBO;

		// Internal setup logic
		void setupMesh();
	};
}
