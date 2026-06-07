#pragma once

#include "../pch.h"
#include "texture.h"
#include "element.h"
#include "mesh.h"

namespace nelems
{
	class Model : public Element
	{
	public:
		// model data
		std::vector<Texture>	textures_loaded;
		std::vector<Mesh>		meshes;
		std::string				directory;
		bool					gammaCorrection;

		// constructor, expects a filepath to a 3D model
		Model(std::string const& path, bool gamma = false) : gammaCorrection(gamma)
		{
			loadModel(path);
		}

		~Model() { std::cout << "model ahs been dleeted\n"; }

		void Draw(nshaders::Shader* shader);

		void update(nshaders::Shader* shader) override;

		// variables to modify
		float mRoughness;
		float mMetallic;

	private:
		// load a model with supported ASSIMP extensions from file and stores 
		void loadModel(std::string const& path);

		// processes nodes in a recursive fashion
		void processNode(aiNode* node, const aiScene* scene);

		// processes mesh
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);

		// checks all material textures of a given type and loads the textures if they're not loaded yet
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	};
}
