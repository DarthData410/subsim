#include "objekt.hpp"
#include <zufall.hpp>

Objekt::Objekt(const Ogre::Vector3& pos,
               const Ogre::Quaternion& orientation) :
               pos(pos),
               orientation(orientation)
{
    regenerate_id();
}

Objekt::~Objekt() {

}

void Objekt::regenerate_id() {
    id = Zufall::random_hash<decltype(id)>(); // TODO check if exists
}

float Objekt::get_bearing() const {
    return 180.f - orientation.getYaw(true).valueDegrees();
}

float Objekt::get_pitch() const {
    return orientation.getPitch().valueDegrees();
}
