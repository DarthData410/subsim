#include "physik.hpp"

void Physik::rotate2d(rp3d::Vector3& v, float angle) {
    const auto x = v.x;
    const auto z = v.z;
    v.x = x * std::cos(angle) - z * std::sin(angle);
    v.z = x * std::sin(angle) + z * std::cos(angle);
}
