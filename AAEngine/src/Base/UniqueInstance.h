#pragma once
#include <iostream>
namespace AA{
class UniqueInstance
{
private:
  int UniqueID;
public:
  int GetUID() noexcept;
  UniqueInstance();
};
}  // end namespace AA
