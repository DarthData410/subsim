#include "nada/random.hpp"

#include <cfloat>

template float nada::random::get<float>(float a, float b);
template double nada::random::get<double>(double a, double b);
template long double nada::random::get<long double>(long double a, long double b);
template char nada::random::get<char>(char a, char b);
template unsigned char nada::random::get<unsigned char>(unsigned char a, unsigned char b);
template int nada::random::get<int>(int a, int b);
template unsigned nada::random::get<unsigned>(unsigned a, unsigned b);
template long nada::random::get<long>(long a, long b);
template unsigned long nada::random::get<unsigned long>(unsigned long a, unsigned long b);
template long long nada::random::get<long long>(long long a, long long b);
template unsigned long long nada::random::get<unsigned long long>(unsigned long long a, unsigned long long b);

bool nada::random::b(unsigned int chance) { return chance >= ui(1, 100); }

int nada::random::i(int a, int b) { return get<int>(a, b); }

long long nada::random::ll(long long a, long long b) { return get<long long>(a, b); }

unsigned int nada::random::ui(unsigned int a, unsigned int b) { return get<unsigned int>(a, b); }

unsigned long nada::random::ul(unsigned long a, unsigned long b) { return get<unsigned long>(a, b); }

float nada::random::f(float a, float b) { return get<float>(a, b); }

double nada::random::d(double a, double b) { return get<double>(a, b); }

template<typename T, std::enable_if_t<std::is_integral<T>::value, int>>
T nada::random::get(T a, T b) {
    thread_local std::random_device rd;
    thread_local std::mt19937 mt(rd());
    std::uniform_int_distribution<T> uid(a, b);
    return uid(mt);
}

template<typename T, std::enable_if_t<std::is_floating_point<T>::value, int>>
T nada::random::get(T a, T b) {
    thread_local std::random_device rd;
    thread_local std::mt19937 mt(rd());
    std::uniform_real_distribution<double> urd(a, std::nextafter(b, DBL_MAX));
    return urd(mt);
}
