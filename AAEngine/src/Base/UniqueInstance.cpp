#include "UniqueInstance.h"
#include <iostream>
static int uid;
namespace AA {
int UniqueInstance::GetUID() noexcept { return UniqueID; }
UniqueInstance::UniqueInstance() {
  uid = 0;
  UniqueID = ++uid;
  std::cout << "UID created: " << UniqueID << '\n';
};
}