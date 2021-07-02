#include "Skybox.h"
#include "../../include/AncientArcher/Utility/Files.h"
#include "../Renderer/OpenGL/OGLGraphics.h"
#include "../Renderer/LoadCube.h"
#include "../Settings/Settings.h"
#include <stb_image.h>
#include "../Utility/QueryShader.h"
#include "../GLSL/Raw_Shaders.h"
namespace AA{
Skybox::Skybox(std::vector<std::string> incomingSkymapFiles) {

  mSkyboxShader = std::make_unique<OGLShader>(
    vert_skycube.c_str(),
    frag_skycube.c_str()
  );
#ifdef _DEBUG
  QueryInputAttribs(mSkyboxShader->GetID());
  QueryUniforms(mSkyboxShader->GetID());
#endif
  mSkyboxVAO = load_cube();

  // todo: load up debug box if skymap files are wrong

  int width, height, nrChannel;
  stbi_set_flip_vertically_on_load(0); // tell stb_image.h to not flip loaded texture's on the y-axis.
  std::vector<unsigned char*> data;
  data.resize(6);
  for (auto i = 0; i < 6; ++i)
    data[i] = stbi_load(incomingSkymapFiles[i].c_str(), &width, &height, &nrChannel, STBI_rgb_alpha );
  mCubemapTexId = OGLGraphics::UploadCubeMapTex(data, width, height, nrChannel == 4);
  mSkyboxShader->Use();
  mSkyboxShader->SetInt("skybox", 0);
}

void Skybox::Render() {
  mSkyboxShader->Use();
  OGLGraphics::SetSamplerCube(0, mCubemapTexId);
  OGLGraphics::RenderSkybox(mSkyboxVAO, 36);
}

void Skybox::SetProjectionMatrix(glm::mat4 proj_mat) {
  mSkyboxShader->Use();
  mSkyboxShader->SetMat4("projection", proj_mat);
}

void Skybox::SetViewMatrix(glm::mat4 view_mat) {
  // turn the last column into 0's
  const glm::mat4 viewMatrix = glm::mat4(glm::mat3(view_mat));
  mSkyboxShader->Use();
  mSkyboxShader->SetMat4("view", viewMatrix);
}

}  // end namespace AA