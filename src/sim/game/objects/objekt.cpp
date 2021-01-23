#include "objekt.hpp"
#include <zufall.hpp>

Objekt::Objekt(const Ogre::Vector3& pos,
               const Ogre::Quaternion& orientation) :
               pos(pos),
               orientation(orientation)
{
    // ID zuweisen, wiederholen falls doppelt
    do id = Zufall::get<decltype(id)>(0, std::numeric_limits<decltype(id)>::max());
    while (given_ids.count(id) != 0);
    given_ids.insert(id);
}

Objekt::~Objekt() {
    given_ids.erase(id);
}

float Objekt::get_bearing() const {
    return orientation.getYaw(true).valueDegrees() + 180.f;
}

float Objekt::get_pitch() const {
    return orientation.getPitch().valueDegrees();
}
