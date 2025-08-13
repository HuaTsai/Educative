// calculator.cc
#include <calculator.inl>

template double Calculator::add<double>(double a, double b);
template int Calculator::add<int>(int a, int b);
template float Calculator::add<float>(float a, float b);
