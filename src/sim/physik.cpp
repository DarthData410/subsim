#include "physik.hpp"

#include <cmath>

void Physik::move_ahead(Vektor& pos, double bearing, double pitch, float speed) {
    // TODO
}

float Physik::rotation(float winkel1, float winkel2) {
    const float diff = winkel2 - winkel1;
    auto modulo = [](float a, float m) { return fmodf((fmodf(a, m) + m), m); };
    return modulo(diff + 180.f, 360.f) - 180.f;
}

float Physik::bearing(float x, float y, float target_x, float target_y) {
    return std::atan2(target_x-x, y-target_y) * 180.f / static_cast<float>(M_PI);
}

float Physik::winkel_tiefe(const Vektor& pos, const Vektor& target_pos) {
    const float d = Physik::distanz(pos.x(), pos.y(), target_pos.x(), target_pos.y());
    return bearing(0.f, pos.z(), d, target_pos.z());
}

double Physik::distanz(float x1, float y1, float x2, float y2) {
    return std::hypot(x2-x1, y2-y1);
}

double Physik::distanz_xy(const Vektor& v1, const Vektor& v2) {
    return distanz(v1.x(), v1.y(), v2.x(), v2.y());
}

double Physik::distanz(const Vektor& v1, const Vektor& v2) {
    return hypot(v1.x()-v2.x(), hypot(v1.y()-v2.y(), v1.z()-v2.z()));
}

double Physik::bremsweg(float v, float a) {
    return (v * v) / (2.0f * a);
}

std::array<float,2> Physik::get_punkt(float x, float y, float bearing, float entfernung) {
    const float winkel_rad = (static_cast<float>(M_PI) * (bearing + 90.f)) / 180.f;
    const float px = x + entfernung * std::cos(winkel_rad);
    const float py = y + entfernung * std::sin(winkel_rad);
    return {px,py};
}
