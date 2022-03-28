#include "physik.hpp"

#include <cmath>

void Physik::move(Vektor& pos, float kurs, double weite) {
    const auto& neue_pos = get_punkt(pos.x(), pos.y(), kurs, weite);
    pos.x(neue_pos.first);
    pos.y(neue_pos.second);
}

float Physik::winkel_diff(float winkel1, float winkel2) {
    const float diff = winkel2 - winkel1;
    auto modulo = [](float a, float m) { return fmodf((fmodf(a, m) + m), m); };
    return modulo(diff + 180.f, 360.f) - 180.f;
}

float Physik::kurs(float x, float y, float target_x, float target_y) {
    return fmodf(std::atan2(target_x-x, target_y-y) * 180.f / static_cast<float>(M_PI) + 360.f, 360.f);
}

float Physik::winkel_tiefe(const Vektor& pos, const Vektor& target_pos) {
    const float d = static_cast<float>(Physik::distanz(pos.x(), pos.y(), target_pos.x(), target_pos.y()));
    const float k = kurs(0.f, 0.f, d, pos.z()-target_pos.z());
    return winkel_diff(90.f, k);
}

double Physik::distanz(double x1, double y1, double x2, double y2) {
    return std::hypot(x2-x1, y2-y1);
}

double Physik::distanz_xy(const Vektor& v1, const Vektor& v2) {
    return distanz(v1.x(), v1.y(), v2.x(), v2.y());
}

double Physik::distanz_xyz(const Vektor& v1, const Vektor& v2) {
    return hypot(v1.x()-v2.x(), hypot(v1.y()-v2.y(), v1.z()-v2.z()));
}

double Physik::bremsweg(double v, double a) {
    return std::abs((v * v) / (2.0 * a));
}

std::pair<double, double> Physik::get_punkt(double x, double y, float kurs, double entfernung) {
    const float winkel_rad = (static_cast<float>(M_PI) * (kurs) / 180.f);
    const double px = x + entfernung * std::sin(winkel_rad);
    const double py = y + entfernung * std::cos(winkel_rad);
    return {px,py};
}

bool Physik::in_reichweite_xy(const Vektor& v1, const Vektor& v2, double reichweite) {
    if (std::abs(v1.x() - v2.x()) > reichweite) return false;
    if (std::abs(v1.y() - v2.y()) > reichweite) return false;
    return distanz_xy(v1, v2) <= reichweite;
}
