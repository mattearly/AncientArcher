#include "UniqueInstance.h"
#include <iostream>
static int uid = 0;
namespace AA {
int UniqueInstance::GetUID() noexcept { return UniqueID; }
UniqueInstance::UniqueInstance() {
  UniqueID = ++uid;
  std::cout << "UID created: " << UniqueID << '\n';
};
}