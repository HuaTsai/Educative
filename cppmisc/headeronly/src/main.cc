// main.cc
#include <calculator.hpp>
#include <print>

int main() {
  Calculator calc;

  std::print("Calculator Examples:\n");
  std::print("===================\n");

  // Template function examples
  std::print("Template add function:\n");
  std::print("  add(5, 3) = {}\n", calc.add(5, 3));
  std::print("  add(2.5, 1.7) = {}\n", calc.add(2.5, 1.7));
  std::print("  add(1.5f, 2.3f) = {}\n", calc.add(1.5f, 2.3f));

  // Member functions
  std::print("\nMember functions:\n");
  std::print("  multiply(4.0, 5.0) = {}\n", calc.multiply(4.0, 5.0));
  std::print("  square(7.0) = {}\n", calc.square(7.0));

  // Free function
  std::print("\nFree function:\n");
  std::print("  power(2.0, 8) = {}\n", power(2.0, 8));
  std::print("  power(3.0, -2) = {}\n", power(3.0, -2));
}