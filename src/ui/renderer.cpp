#include "renderer.h"
#include "../elems/vertex.h"
#include "../elems/mesh.h"
#include "../elems/texture.h"


void Renderer::Draw(nshaders::Shader* shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 2.0f));
    shader->setMat4("model", model);
    renderCube(shader);
}

// -------------------------------------------------
void Renderer::renderCube(nshaders::Shader* shader)
{
    // 1. Create a tiny dummy triangle or use your cube vertices
    std::vector<Vertex> dummyVertices;
    Vertex v1, v2, v3;
    v1.Position = glm::vec3(-1.0f, -1.0f, 0.0f);
    v2.Position = glm::vec3(1.0f, -1.0f, 0.0f);
    v3.Position = glm::vec3(0.0f, 1.0f, 0.0f);
    dummyVertices.push_back(v1); dummyVertices.push_back(v2); dummyVertices.push_back(v3);

    std::vector<unsigned int> dummyIndices = { 0, 1, 2 };
    std::vector<Texture> dummyTextures; // Leave empty

    // 2. Feed it into your actual Mesh class manually
    nelems::Mesh testMesh(dummyVertices, dummyIndices, dummyTextures);

    testMesh.Draw(shader);
}
