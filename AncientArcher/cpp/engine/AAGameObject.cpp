#include "AAGameObject.h"
#include "AAViewport.h"

MeshDrawInfo::MeshDrawInfo(unsigned int a, unsigned int b, unsigned int e) : vao(a), vbo(b), ebo(e) {}

AAGameObject::AAGameObject(std::vector<MeshDrawInfo> meshes, std::vector<TextureInfo> textures, std::vector<unsigned int> elements)
  : mMeshes(meshes), mTextures(textures), mElements(elements) {}

void AAGameObject::draw()
{
    if (mShader == nullptr)
    {
      throw "Shader not set for mesh drawing.";
    }
    else
    {
      mShader->use();
      mShader->setMat4("view", AAViewport::getInstance()->getViewMatrix());
    }

    for (auto m : mMeshes)
    {
      //todo
    }
}
