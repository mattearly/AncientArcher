#include "../engine/AAEngine.h"
#include "loadModel.h"
#include <glm/glm.hpp>
int main()
{

  // Test 1 - Loading A Model
  //showModelHeaderInfo("C:\Users\matt\Dropbox_me298414\Dropbox\My3DModels\6ColorSquare.obj");
  showModelHeaderInfo("C:\\Users\\matt\\Dropbox_me298414\\Dropbox\\My3DModels\\6ColorSquare.obj");

  // Test 2 - Main Engine
  AAEngine engine;                                                    // init engine
  AADisplay::getInstance()->setWindowTitle("newtitle");               // test changing window title
  AADisplay::getInstance()->setWindowSize(1024, 768, 500, 250);       // test changing window size
  AADisplay::getInstance()->setWindowClearColor(glm::vec3(0,0,0));    //test changing window clear color

  std::shared_ptr<AAKeyInput> keys;                                   // test setting up keys
  keys = std::make_shared<AAKeyInput>();
  engine.setKeyStruct(keys);

  engine.run();                                                       // test run (main loop)

  return 0;

}