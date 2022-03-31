#include "physik.hpp"

#include <cmath>

void Physik::move(Vektor& pos, winkel_t kurs, dist_t weite) {
    const auto& neue_pos = get_punkt(pos.x(), pos.y(), kurs, weite);
    pos.x(neue_pos.first);
    pos.y(neue_pos.second);
}

winkel_t Physik::winkel_diff(winkel_t winkel1, winkel_t winkel2) {
    const auto diff = winkel2 - winkel1;
    auto modulo = [](winkel_t a, winkel_t m) { return fmodf((fmodf(a, m) + m), m); };
    return modulo(diff + 180.f, 360.f) - 180.f;
}

winkel_t Physik::kurs(dist_t x, dist_t y, dist_t target_x, dist_t target_y) {
    return fmodf(static_cast<winkel_t>(std::atan2(target_x-x, target_y-y)) * 180.f /
                    static_cast<winkel_t>(M_PI) + 360.f,
                    360.f);
}

winkel_t Physik::winkel_tiefe(const Vektor& pos, const Vektor& target_pos) {
    const auto d = Physik::distanz(pos.x(), pos.y(), target_pos.x(), target_pos.y());
    const auto k = kurs(0.f, 0.f, d, pos.z()-target_pos.z());
    return winkel_diff(90.f, k);
}

dist_t Physik::distanz(dist_t x1, dist_t y1, dist_t x2, dist_t y2) {
    return std::hypot(x2-x1, y2-y1);
}

dist_t Physik::distanz_xy(const Vektor& v1, const Vektor& v2) {
    return distanz(v1.x(), v1.y(), v2.x(), v2.y());
}

dist_t Physik::distanz_xyz(const Vektor& v1, const Vektor& v2) {
    return hypot(v1.x()-v2.x(), hypot(v1.y()-v2.y(), v1.z()-v2.z()));
}

dist_t Physik::bremsweg(dist_t v, dist_t a) {
    return std::abs((v * v) / (2.0 * a));
}

std::pair<dist_t, dist_t> Physik::get_punkt(dist_t x, dist_t y, winkel_t kurs, dist_t entfernung) {
    const auto winkel_rad = (static_cast<winkel_t>(M_PI) * (kurs) / 180.f);
    const auto px = x + entfernung * std::sin(winkel_rad);
    const auto py = y + entfernung * std::cos(winkel_rad);
    return {px,py};
}

bool Physik::in_reichweite_xy(const Vektor& v1, const Vektor& v2, dist_t reichweite) {
    if (std::abs(v1.x() - v2.x()) > reichweite) return false;
    if (std::abs(v1.y() - v2.y()) > reichweite) return false;
    return distanz_xy(v1, v2) <= reichweite;
}

winkel_t Physik::kurs(const Vektor& v, const Vektor& v_target) {
    return kurs(v.x(), v.y(), v_target.x(), v_target.y());
}

bool Physik::is_winkel_zwischen(winkel_t winkel, winkel_t min, winkel_t max) {
    min -= winkel;
    max -= winkel;
    winkel_norm(min);
    winkel_norm(max);
    if (min * max >= 0 ) return false;
    return std::fabs(min - max) < 180;
}

void Physik::winkel_norm(winkel_t& w) {
    while (w < -180) w += 360;
    while (w >  180) w -= 360;
}

float Physik::schallfaktor(dist_t d) {
    const float df = static_cast<float>(d);
    const float d2 = df * df;
    return 1.f / (1.f + 0.000000002f * d2);
}

float Physik::sichtbarkeit(float s, float v, dist_t d) {
    const auto entfernungsfaktor = schallfaktor(d);
    const float sichtbarkeitseinfluss = 3;
    const float x_verschiebung = -3;
    const float x_faktor = 0.25;
    const float y_streckung = 2.5;
    const float y_verschibung = 0.2;
    const float steigungsglaettung = 0.15;
    const float sichtbarkeit = entfernungsfaktor * y_streckung *
          (y_verschibung + (steigungsglaettung *
          std::atan(((x_faktor * std::abs(v)) + x_verschiebung) + (sichtbarkeitseinfluss * s))));
    return sichtbarkeit;
}
