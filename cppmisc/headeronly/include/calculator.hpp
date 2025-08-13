// calculator.hpp
#pragma once

#ifdef CALCULATOR_HEADER_ONLY
#define CALCULATOR_INLINE inline
#else
#define CALCULATOR_INLINE
#endif

class Calculator {
 public:
  // Template Function Declaration
  template <typename T> T add(T a, T b);

  // Member Functions
  CALCULATOR_INLINE double multiply(double a, double b);
  CALCULATOR_INLINE double square(double x);

 private:
  int call_count_{0};
};

// Inline Free Function
CALCULATOR_INLINE double power(double base, int exp);

// Implementation in Header File
#ifdef CALCULATOR_HEADER_ONLY
#include <calculator_impl.hpp>
#endif
