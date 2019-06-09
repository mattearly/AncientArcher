#include "HealthBar.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

HealthBar::HealthBar()
{
  // init shader for rendering health bars
  healthBarShader = std::make_unique<Shader>(
    "../AncientArcher/cpp/pckgs/GUI/health.vert",
    "../AncientArcher/cpp/pckgs/GUI/health.frag"
    );


  // init a rect for showing health bars
  std::vector<float> vertices = {
    -0.5f,  0.5f,  // Top-left
     0.5f,  0.5f,  // Top-right
     0.5f, -0.5f,  // Bottom-right
    -0.5f, -0.5f,  // Bottom-left
  };

  std::vector<unsigned int> elements = {
      0, 1, 2,
      2, 3, 0
  };

  glGenVertexArrays(1, &healthBarVAO);
  glGenBuffers(1, &healthBarVBO);

  glBindBuffer(GL_ARRAY_BUFFER, healthBarVBO);
  glBufferData(GL_ARRAY_BUFFER, (unsigned int)vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  glBindVertexArray(healthBarVAO);

  // position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

  // unbind buffers
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glBindVertexArray(0);

  glGenBuffers(1, &healthBarIBO);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, healthBarIBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  indexSize = (unsigned int)elements.size();

}

HealthBar::~HealthBar()
{
}

void HealthBar::render()
{
  healthBarShader->use();

  glBindVertexArray(healthBarVAO);

  //glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, healthBarIBO);

  glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_INT, (void*)0);

  glBindVertexArray(0);

  static unsigned int k = 0;
  for (k = 0; k < bar1current; ++k){
    // update model
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(1.f - 1.f*k, 1.f, 0.f));
    healthBarShader->setMat4("model", model);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }
  static unsigned int l = 0;
  for (l = 0; l < bar1current; ++l) {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(1.f * k * -1.f, 1.f, 0.f));
    healthBarShader->setMat4("model", model);    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

}

Shader* HealthBar::getShader()
{
  return healthBarShader.get();
}

void HealthBar::setBar1(unsigned int current)
{
  bar1current = current;
  
}

void HealthBar::setBar2(unsigned int current)
{
  bar2current = current;
}
