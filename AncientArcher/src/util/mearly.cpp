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

  bool BBC(const Impasse &a, const Impasse &b)
  {
    float box_a_righ = a.loc[0] + a.sz[0] / 2.f;
    float box_a_left = a.loc[0] - a.sz[0] / 2.f;
    float box_b_left = b.loc[0] - b.sz[0] / 2.f;
    float box_b_righ = b.loc[0] + b.sz[0] / 2.f;
    // right side of box A *could* collide with box B          // left side of box A *could* collide with box B
    if (box_a_righ > box_b_left && box_a_righ < box_b_righ || box_a_left > box_b_left && box_a_left < box_b_righ) {
      float box_a_top = a.loc[1] + a.sz[1] / 2.f;
      float box_a_bot = a.loc[1] - a.sz[1] / 2.f;
      float box_b_top = b.loc[1] + b.sz[1] / 2.f;
      float box_b_bot = b.loc[1] - b.sz[1] / 2.f;
      // top side of box could be inside box b            // bottom side of box could be inside box b
      if (box_a_top > box_b_bot && box_a_top < box_b_top || box_a_bot > box_b_bot && box_a_bot < box_b_top) {
        float box_a_back = a.loc[2] + a.sz[2] / 2.f;
        float box_a_fron = a.loc[2] - a.sz[2] / 2.f;
        float box_b_back = b.loc[2] + b.sz[2] / 2.f;
        float box_b_fron = b.loc[2] - b.sz[2] / 2.f;
        // back of box a could be inside box b                 // front of box a could be inside box b
        if (box_a_back > box_b_fron && box_a_back < box_b_back || box_a_fron > box_b_fron && box_a_fron < box_b_back) {
          return true;
        }
      }
    }
    return false;
  }

  /* end namespace mearly */
}
