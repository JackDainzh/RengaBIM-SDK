/*  Sample: MepCirclesConstructor
 *
 *  This sample illustrates how to construct mep circles in Renga.
 *
 *  Copyright Renga Software LLC, 2024. All rights reserved.
 */

#pragma once
#include <cmath>


namespace
{
  constexpr double eps = 0.001;
}

inline bool isEqual(double a, double b)
{
  return std::abs(a - b) < eps;
};

inline bool lessOrEqual(double a, double b)
{
  return isEqual(a, b) ? true : a < b;
};
