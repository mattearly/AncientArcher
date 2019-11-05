#include "../engine/AAEngine.h"

int main()
{
  // init engine
  AAEngine engine;

  // test changing window title
  AADisplay::getInstance()->setWindowTitle("newtitle");
  
  // test setting up keys
  std::shared_ptr<AAKeyInput> keys;
  keys = std::make_shared<AAKeyInput>();
  engine.setKeyStruct(keys);
  
  // test run (main loop)
  engine.run();

  return 0;
}