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

  // returns an interger between n and k randomly
  int NTKR(int n, int k) {
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    static std::mt19937 mgen(ss);

    std::uniform_int_distribution<std::mt19937::result_type> ntkd(n, k);
    return ntkd(mgen);
  }

  float NTKR(float n, float k) {
    static uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    static std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
    static std::mt19937 mgen(ss);

    std::uniform_real_distribution<float> ntkd(n, k);
    return ntkd(mgen);
  }


}