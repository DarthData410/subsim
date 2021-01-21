#include "zufall.hpp"
#include <cfloat>

template float Zufall::get<float>(float a, float b);
template double Zufall::get<double>(double a, double b);
template long double Zufall::get<long double>(long double a, long double b);
template char Zufall::get<char>(char a, char b);
template unsigned char Zufall::get<unsigned char>(unsigned char a, unsigned char b);
template int Zufall::get<int>(int a, int b);
template unsigned Zufall::get<unsigned>(unsigned a, unsigned b);
template long Zufall::get<long>(long a, long b);
template unsigned long Zufall::get<unsigned long>(unsigned long a, unsigned long b);
template long long Zufall::get<long long>(long long a, long long b);
template unsigned long long Zufall::get<unsigned long long>(unsigned long long a, unsigned long long b);

bool Zufall::b(unsigned int chance) { return chance >= ui(1, 100); }

int Zufall::i(int a, int b) { return get<int>(a, b); }

long long Zufall::ll(long long a, long long b) { return get<long long>(a, b); }

unsigned int Zufall::ui(unsigned int a, unsigned int b) { return get<unsigned int>(a, b); }

unsigned long Zufall::ul(unsigned long a, unsigned long b) { return get<unsigned long>(a, b); }

float Zufall::f(float a, float b) { return get<float>(a, b); }

double Zufall::d(double a, double b) { return get<double>(a, b); }

template<typename T, std::enable_if_t<std::is_integral<T>::value, int>>
T Zufall::get(T a, T b) {
    thread_local std::random_device rd;
    thread_local std::mt19937 mt(rd());
    std::uniform_int_distribution<T> uid(a, b);
    return uid(mt);
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, int>>
T Zufall::get(T a, T b) {
    thread_local std::random_device rd;
    thread_local std::mt19937 mt(rd());
    std::uniform_real_distribution<double> urd(a, std::nextafter(b, DBL_MAX));
    return urd(mt);
}
