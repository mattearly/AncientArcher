//#include "TopDownPlayer.h"
//#include <TextureLoader.h>
//#include <iostream>
////extern Camera g_camera;
//extern Controls g_controls;
//
//void TopDownPlayer::init()
//{
//  TextureLoader texloader;
//  
//  unsigned int spaceshipTexture = texloader.load2DTexture("../AncientArcher/cpp/pckgs/topDownPlayer/chronos-javelin.jpg");
//  
//  _playerShip = std::make_unique<Entity>(
//    ENTITYTYPE::PLANE,
//    glm::vec3(0,1,0),
//    glm::vec3(1.f, 0.25f, 1.f),
//    spaceshipTexture,
//    true,
//    false
//  );
//
//  float vertices[] = {
//    // positions               // normals       //text cords    
//    -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f,
//     0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    1.0f, 1.0f,
//     0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
//     0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f,
//    -0.5f, 0.0f, 0.5f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f,
//    -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,    0.0f, 1.0f
//  };
//
//
//  /* set up an area to store vertex data */
//  unsigned int planeVBO = 0;
//  glGenVertexArrays(1, &planeVAO);
//  glGenBuffers(1, &planeVBO);
//
//  glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//  glBindVertexArray(planeVAO);
//
//  // position attribute
//  glEnableVertexAttribArray(0);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//
//  // normal attribute
//  glEnableVertexAttribArray(2);
//  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//
//  // texture coord attribute
//  glEnableVertexAttribArray(1);
//  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//
//  //glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//  glBindVertexArray(0);
//
//
//}
//
//void TopDownPlayer::syncCam()
//{
//  //g_camera.setPosition(_entity->gameItem.loc + _camOffset);
//}
//
//TopDownPlayer::TopDownPlayer()
//{
//  init();
//}
//
//TopDownPlayer::~TopDownPlayer()
//{
//}
//
//void TopDownPlayer::moveRight(float amount)
//{
//  g_camera.Position.x += amount;
//  std::cout << "pos( x:" << g_camera.getPosition()->x << " z:" << g_camera.getPosition()->z << std::endl;
//}
//
//void TopDownPlayer::moveUp(float amount)
//{
//  g_camera.Position.z += amount;
//}
//
//void TopDownPlayer::update()
//{
//  //g_controls.tdpKeyboardIn(_moveStatus);
//}
//
//void TopDownPlayer::updateMovement(float deltaTime)
//{
//  static const float MVSPEED = 1.55666667f;
//  if (_moveStatus.up)
//  {
//    moveUp(-MVSPEED * deltaTime);
//  }
//  if (_moveStatus.down)
//  {
//    moveUp(MVSPEED * deltaTime);
//  }
//  if (_moveStatus.left)
//  {
//    moveRight(-MVSPEED * deltaTime);
//  }
//  if (_moveStatus.right)
//  {
//    moveRight(MVSPEED * deltaTime);
//  }
//
//  //syncCam();
//}
//
//void TopDownPlayer::render(Shader *shader)
//{
//  shader->use();
//  //g_camera.update(shader);
//
//  glActiveTexture(GL_TEXTURE0);
//  glBindTexture(GL_TEXTURE_2D, _playerShip->gameItem.textureID);
//
//  glm::mat4 model = glm::mat4(1.0f);
//  // step1: translate
//  model = glm::translate(model, glm::vec3(_playerShip->gameItem.loc));
//  // step2: rotations  -- not supported by colliders yet
//    //model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
//    //model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
//    //model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//  // step3: scale
//  model = glm::scale(model, glm::vec3(_playerShip->gameItem.scale));
//
//  shader->setMat4("model", model);
//
//  glBindVertexArray(planeVAO);
//  glEnableVertexAttribArray(0);
//  glDrawArrays(GL_TRIANGLES, 0, 6);
//  glBindVertexArray(0);
//
//}
