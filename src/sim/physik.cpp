#include "physik.hpp"

#include <Ogre.h>

#include <cmath>

void Physik::set_pitch(Ogre::Quaternion& q, float target_pitch) {
    assert(false && "Physik::set_pitch Muss repariert werden."); // TODO kaputt
    const auto pitch_now = q.getPitch().valueDegrees();
    const auto pitch = target_pitch - pitch_now;
    if (std::abs(pitch) < 0.001f) return;
    q = q * Ogre::Quaternion(Ogre::Degree(pitch), Ogre::Vector3::UNIT_X);
    q.normalise();
    //const float s = std::sin(target_pitch);
    //const float c = std::cos(target_pitch);

}

void Physik::rotate(Ogre::Quaternion& q, float degree) {
    if (std::abs(degree) < 0.001f) return;
    q = q * Ogre::Quaternion(Ogre::Degree(-degree), Ogre::Vector3::UNIT_Y);
    q.normalise();
}

void Physik::move_ahead(Ogre::Vector3& pos, const Ogre::Quaternion& q, float amount) {
    pos += q * Ogre::Vector3(0, 0, amount);
}

float Physik::rotation(float winkel1, float winkel2) {
    const float diff = winkel2 - winkel1;
    auto modulo = [](float a, float m) { return fmodf((fmodf(a, m) + m), m); };
    return modulo(diff + 180.f, 360.f) - 180.f;
}

float Physik::bearing(float x, float y, float target_x, float target_y) {
    return std::atan2(target_x-x, y-target_y) * 180.f / static_cast<float>(M_PI);
}

float Physik::winkel_tiefe(const Ogre::Vector3& pos, const Ogre::Vector3& target_pos) {
    const float d = Physik::distanz(pos.x, pos.z, target_pos.x, target_pos.z);
    return bearing(0.f, pos.y, d, target_pos.y);
}

float Physik::distanz(float x1, float y1, float x2, float y2) {
    return std::hypot(x2-x1, y2-y1);
}

float Physik::distanz(const Ogre::Vector3& pos, const Ogre::Vector3& target_pos) {
    return pos.distance(target_pos);
}

float Physik::bremsweg(float v, float a) {
    return (v * v) / (2.0f * a);
}

std::array<float,2> Physik::get_punkt(float x, float y, float bearing, float entfernung) {
    const float winkel_rad = (static_cast<float>(M_PI) * (bearing + 90.f)) / 180.f;
    const float px = x + entfernung * std::cos(winkel_rad);
    const float py = y + entfernung * std::sin(winkel_rad);
    return {px,py};
}
