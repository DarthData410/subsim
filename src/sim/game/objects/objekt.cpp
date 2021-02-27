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
    std::scoped_lock lock(objekt_mutex);
    given_ids.erase(id);
}

void Objekt::regenerate_id() {
    std::scoped_lock lock(objekt_mutex);
    while (given_ids.count(id)) [[unlikely]] id = Zufall::random_hash<decltype(id)>();
    given_ids.insert(id);
}

float Objekt::get_bearing() const {
    return 180.f - orientation.getYaw(true).valueDegrees();
}

float Objekt::get_pitch() const {
    return orientation.getPitch().valueDegrees();
}
