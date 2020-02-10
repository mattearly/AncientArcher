#pragma once

namespace mearly
{

#define coinflip fiftyfifty
#define fiftyfifty Random::ZOOR()

class Random
{
public:

  // zero to one
  static double ZTOR();

  // zero or one
  static int ZOOR();

  // n to k
  static int NTKR(int n, int k);
  static float NTKR(float n, float k);
  static double NTKR(double n, double k);

private:
};

}  // end namespace mearly