#include <iostream>
#include "Random.h"

int main()
{
  int result = Random::NTKR(1, 20);

  std::cout << "Result: " << result << '\n';
  
  char a;
  std::cin >> a;

  return 0;
}