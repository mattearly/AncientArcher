#include "mearly.h"

namespace mearly {

  // returns a double between zero and one randomly
  double ZTOR() {
    // initialize the random number generator with time-dependent seed
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    static std::mt19937 mgen(ss);
    // initialize a uniform distribution between 0 and 1
    static std::uniform_real_distribution<double> unif(0, 1);
    return unif(mgen);
  }

  // returns integer zero or one randomly [true / false]
  int ZOOR() {
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    static std::mt19937 mgen(ss);
    // initialize a distribution of 0 or 1 randomer
    static std::uniform_int_distribution<std::mt19937::result_type> dist2(0, 1);
    return dist2(mgen);
  }

  // returns a interger between n and k randomly
  int NTKR(int n, int k) {
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    static std::mt19937 mgen(ss);

    std::uniform_int_distribution<std::mt19937::result_type> ntkd(n, k);
    return ntkd(mgen);
  }

  // returns a float between n and k randomly
  float NTKR(float n, float k) {
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    static std::mt19937 mgen(ss);

    std::uniform_real_distribution<float> ntkd(n, k);
    return ntkd(mgen);
  }

  bool AABB_vs_AABB_3D(const Impasse &a, const Impasse &b)
  {

    float aMinX = a.loc[0] - a.sz[0] / 2.f;
    float aMaxX = a.loc[0] + a.sz[0] / 2.f;
    float aMinY = a.loc[1] - a.sz[1] / 2.f;
    float aMaxY = a.loc[1] + a.sz[1] / 2.f;
    float aMinZ = a.loc[2] - a.sz[2] / 2.f;
    float aMaxZ = a.loc[2] + a.sz[2] / 2.f;

    float bMinX = b.loc[0] - b.sz[0] / 2.f;
    float bMaxX = b.loc[0] + b.sz[0] / 2.f;
    float bMinY = b.loc[1] - b.sz[1] / 2.f;
    float bMaxY = b.loc[1] + b.sz[1] / 2.f;
    float bMinZ = b.loc[2] - b.sz[2] / 2.f;
    float bMaxZ = b.loc[2] + b.sz[2] / 2.f;


    return (aMinX <= bMaxX && aMaxX >= bMinX) && (aMinY <= bMaxY && aMaxY >= bMinY) && (aMinZ <= bMaxZ && aMaxZ >= bMinZ);
  }

  /* end namespace mearly */
}
