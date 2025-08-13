// calculator_impl.hpp
#pragma once

#ifndef CALCULATOR_HEADER_ONLY
#include <calculator.hpp>
#endif

template <typename T>
T Calculator::add(T a, T b) {
  return a + b;
}

double Calculator::multiply(double a, double b) {
  ++call_count_;
  return a * b;
}

double Calculator::square(double x) {
  return x * x;
}

double power(double base, int exp) {
  if (exp == 0) {
    return 1.0;
  }
  if (exp < 0) {
    return 1.0 / power(base, -exp);
  }

  double result = 1.0;
  for (int i = 0; i < exp; ++i) {
    result *= base;
  }
  return result;
}
