#pragma once
// roll and randomization library using int, float, and double
namespace mearly
{
#define r Random  // class alias in this namespace

// real rolls - dice replicants
#define roll1d2   Random::ZOOR()
#define roll1d3   Random::NTKR(1, 3)
#define roll1d4   Random::NTKR(1, 4)
#define roll1d6   Random::NTKR(1, 6)
#define roll1d8   Random::NTKR(1, 8)
#define roll1d12  Random::NTKR(1, 12)
#define roll1d20  Random::NTKR(1, 20)
#define roll1d30  Random::NTKR(1, 30)
#define roll1d34  Random::NTKR(1, 34)
#define roll1d40  Random::NTKR(1, 40)
#define roll1d60  Random::NTKR(1, 60)
#define roll1d100 Random::NTKR(1, 100)
#define roll1d120 Random::NTKR(1, 120)
#define roll1d400 Random::NTKR(1, 400)

// unatural rolls
#define roll1d1000      Random::NTKR(1, 1000)      // 1 to 1,000   Thousand
#define roll1d10000     Random::NTKR(1, 10000)     // 1 to 10,000  Ten Thousand
#define roll1d100000    Random::NTKR(1, 100000)    // 1 to 100,000  Hundred Thousand
#define roll1d1000000   Random::NTKR(1, 100000)    // 1 to 1,000,000  Million
#define roll1d10000000  Random::NTKR(1, 1000000)   // 1 to 10,000,000  Ten Million
#define roll1d100000000 Random::NTKR(1, 10000000)  // 1 to 100,000,000  Hundred Million

// further aliases
#define coinflip fiftyfifty
#define fiftyfifty rolld2

#define rolld2   roll1d2
#define rolld3   roll1d3
#define rolld4   roll1d4
#define rolld6   roll1d6
#define rolld8   roll1d8
#define rolld12  roll1d12
#define rolld20  roll1d20
#define rolld30  roll1d30
#define rolld34  roll1d34
#define rolld40  roll1d40
#define rolld60  roll1d60
#define rolld100 roll1d100
#define rolld120 roll1d120
#define rolld400 roll1d400

#define rolld1000      roll1d1000
#define rolld10000     roll1d10000
#define rolld100000    roll1d100000
#define rolld1000000   roll1d1000000
#define rolld10000000  roll1d10000000
#define rolld100000000 roll1d100000000

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