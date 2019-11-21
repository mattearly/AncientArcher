#pragma once

class Random
{
public:
  static Random* getInstance();
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
private:
};