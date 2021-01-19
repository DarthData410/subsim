#include "physik.hpp"

#include <Ogre.h>

void Physik::set_pitch(Ogre::Quaternion& q, float target_pitch) {
    const auto pitch_now = q.getPitch().valueDegrees();
    const auto pitch = target_pitch - pitch_now;
    if (std::abs(pitch) < 0.001f) return;
    q = q * Ogre::Quaternion(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X);
    q.normalise();
    //const float s = std::sin(target_pitch);
    //const float c = std::cos(target_pitch);

}

void Physik::rotate(Ogre::Quaternion& q, float degree) {
    if (degree < 0.001f) return;
    q = q * Ogre::Quaternion(Ogre::Degree(degree), Ogre::Vector3::NEGATIVE_UNIT_Z);
    q.normalise();
}

void Physik::move_ahead(Ogre::Vector3& pos, const Ogre::Quaternion& q, float amount) {
    pos += q * (Ogre::Vector3::UNIT_Y * amount);
}
