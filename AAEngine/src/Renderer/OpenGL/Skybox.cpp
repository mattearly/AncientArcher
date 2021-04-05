#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../../Renderer/OpenGL/Skybox.h"
#include "../../Renderer/OpenGL/OGLGraphics.h"
#include "../../Settings/Settings.h"
#include <stb_image.h>

namespace AA
{
/**
 * Custom skymap constructor. Loads up the files in the path and skymap shader.
 * @param incomingSkymapFiles  A six png image cube map texture. The order must be: "right", "left", "up", "down", "front", "back"
 */
Skybox::Skybox(std::vector<std::string> incomingSkymapFiles, bool useInternalShaders, const char* vertpath, const char* fragpath)
{
  if (useInternalShaders) {
    const char* skycubevert =
      "#version 430 core\n"
      "layout(location = 0) in vec3 aPos;\n"
      "out vec3 TexCoords;\n"
      "uniform mat4 projection;\n"
      "uniform mat4 view;\n"
      "void main(){\n"
      "  TexCoords = aPos;\n"
      "  vec4 pos = projection * view * vec4(aPos, 1.0);\n"
      "  gl_Position = pos.xyww;\n"
      "}\n"
      ;
    const char* skycubefrag =
      "#version 430 core\n"
      "in vec3 TexCoords;\n"
      "out vec4 FragColor;\n"
      "uniform samplerCube skybox;\n"
      "void main() {\n"
      "  FragColor = texture(skybox, TexCoords);\n"
      "}\n"
      ;
    skyboxShader = std::make_unique<OGLShader>(
      skycubevert,
      skycubefrag
      );
  }
  else {
    if (vertpath == "" || fragpath == "")
      throw("shader path must be provided if not using internal shaders");
    skyboxShader = std::make_unique<OGLShader>(vertpath, fragpath);
  }
  loadSkybox();

  if (incomingSkymapFiles.size() != 6)
  {
    throw("wrong number of cubemap files for skybox");
  }
  else  // is size 6, load and use
  {
    int width, height, nrChannel;
    stbi_set_flip_vertically_on_load(0); // tell stb_image.h to not flip loaded texture's on the y-axis.
    std::vector<unsigned char*> data;
    data.resize(6);
    for (auto i = 0; i < 6; ++i)
      data[i] = stbi_load(incomingSkymapFiles[i].c_str(), &width, &height, &nrChannel, STBI_rgb);
    switch (Settings::Get()->GetOptions().renderer)
    {
    case RenderingFramework::OPENGL:
      cubemapTexture = OGLGraphics::UploadCubeMapTex(data, width, height);
      break;
    }
  }
  skyboxShader->use();
  skyboxShader->setInt("skybox", 0);
}

/**
 * Renders the skybox behind all other visable objects.
 */
void Skybox::render(const Camera& cam)
{
  glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

  loadViewMatrix(cam);  // todo: only update when camera moved

  glBindVertexArray(mSkyboxVAO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  skyboxShader->use();
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS); // set depth function back to default
}

void Skybox::setProjectionMatrix(const Camera& cam)
{
  loadProjectionMatrix(cam);
}

/**
 * Loads up a cube that is pushed to the graphics card. skyboxVAO and skyboxVBO are populated with IDs.
 */
void Skybox::loadSkybox()
{
  const float SIZE = 1.f;

  float skyboxVertices[] = {
    // positions
    -SIZE,  SIZE, -SIZE,
    -SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,

    -SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE, -SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,

     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,

    -SIZE, -SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE, -SIZE,  SIZE,
    -SIZE, -SIZE,  SIZE,

    -SIZE,  SIZE, -SIZE,
     SIZE,  SIZE, -SIZE,
     SIZE,  SIZE,  SIZE,
     SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE,  SIZE,
    -SIZE,  SIZE, -SIZE,

    -SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
     SIZE, -SIZE, -SIZE,
     SIZE, -SIZE, -SIZE,
    -SIZE, -SIZE,  SIZE,
     SIZE, -SIZE,  SIZE
  };

  //todo: use element buffers
  glGenVertexArrays(1, &mSkyboxVAO);

  unsigned int skyboxVBO = 0;

  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(mSkyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

/**
 * Sets the view matrix value on the skyboxShader from the GetViewMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void Skybox::loadViewMatrix(const Camera& cam)
{
	const glm::mat4 viewMatrix = glm::mat4(glm::mat3(cam.GetViewMatrix()));
	skyboxShader->use();
	skyboxShader->setMat4("view", viewMatrix);
}

/**
 * Sets the projection matrix value on the skyboxShader from the GetProjectionMatrix() function in AAViewport.
 * skyboxShader is in use after this call completes.
 */
void Skybox::loadProjectionMatrix(const Camera& cam)
{
	glm::mat4 projectionMatrix = cam.GetProjectionMatrix();
	skyboxShader->use();
	skyboxShader->setMat4("projection", projectionMatrix);
}
}  // end namespace AA