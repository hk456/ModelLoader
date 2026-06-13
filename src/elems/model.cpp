#include "mesh.h"
#include "model.h"

namespace nelems
{
	void Model::Draw(nshaders::Shader* shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

	void Model::update(nshaders::Shader* shader)
	{	
        glm::mat4 model = glm::mat4(1.0f); 
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->setMat4("model", model);
	}

	void nelems::Model::loadModel(std::string const& path)
	{
		// define import flags
		const uint32_t cMeshImportFlags =
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_SortByPType |
			aiProcess_GenNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_ValidateDataStructure;

		// read the file via ASSIMP
		Assimp::Importer importer;

		const aiScene* scene = importer.ReadFile(path, cMeshImportFlags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "Error Assimp: " << importer.GetErrorString() << std::endl;
			return;
		}

		// retrieve the directory path of the filepath
		std::size_t lastSlash = path.find_last_of("\\/");
		this->directory = path.substr(0, lastSlash);

		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// after we've processed all of the meshes of the current node, we then recursively process each of the children node
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill 
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<Texture>		textures;

		// walk through each of the meshes' vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;

				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;

				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;

				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::cout << "[Assimp Debug] Processed a mesh!" << std::endl;
		std::cout << "   - Vertices count: " << vertices.size() << std::endl;
		std::cout << "   - Indices count: " << indices.size() << std::endl;

		if (!vertices.empty()) {
			std::cout << "   - Vertex[0] Position: ("
				<< vertices[0].Position.x << ", "
				<< vertices[0].Position.y << ", "
				<< vertices[0].Position.z << ")" << std::endl;
		}

		return Mesh(vertices, indices, textures);
	}

	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.id = loadTexture(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}
}