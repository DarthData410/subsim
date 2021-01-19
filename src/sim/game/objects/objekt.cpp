#include "objekt.hpp"

Objekt::Objekt(const Ogre::Vector3& pos,
               const Ogre::Quaternion& orientation) :
               pos(pos),
               orientation(orientation)
{
    //
}

float Objekt::get_bearing() const {
    return orientation.getYaw().valueDegrees() + 180.f;
}

float Objekt::get_pitch() const {
    // sin(a) = y / z
    //return std::asin(orientation.y / orientation.z);
    return orientation.getPitch().valueDegrees() + 90.f;
}
