#pragma once

/*
~ Utility Functions
~ built by Matthew Early
*/

#include <random>
#include <chrono>

namespace mearly {

///
/// \brief ZOOR  Zero-Or-One-Random
/// \return      0 or 1
///
int ZOOR();

///
/// \brief NTKR  N-To-K-Random (int, int)
/// \return      a random integer in the range N to K
///
int NTKR(int n, int k);

///
/// \brief NTKR  N-To-K-Random (float, float)
/// \return      a random float in the range N to K
///
float NTKR(float n, float k);

///
/// \brief   ZTOR Zero-To-One-Random uses MersenneTwister seeded on a high precision
///          timestamp seed for a true feeling of randomness
/// \return  double between 0.0 and 1.0
///
double ZTOR();

///
/// for collision
/// considering this article for implementation:
///  https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection
///
struct Impasse {
  float loc[3];
  float sz[3];
};

///
/// \brief AABB_vs_AABB_3D
/// \param[in] a   a blockage
/// \param[in] b   another blockage
/// \returns       true if they collide, returns false if they do not
///
bool AABB_vs_AABB_3D(const Impasse& a, const Impasse& b);

///
/// \brief BBC  BoundingBoxCollision (Impasse, Impasse)
/// \returns    True if they collide, returns false if they do not
///
bool point_vs_AABB_3D(const float& px, const float& py, const float& pz, const Impasse& b);

}
