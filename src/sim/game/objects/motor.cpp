#include "motor.hpp"
#include "../../physik.hpp"

#include <algorithm>

Motor::Motor(float v_max, float a)
: v_max(v_max), v_target(0), v(0), a(a) {}

void Motor::tick(float s) {
    if (std::abs(v-v_target) <= a) v = v_target; // Zielgeschwindigkeit erreicht
    else if (v < v_target) v += a * s; // Beschleunigen
    else if (v > v_target) v -= a * s; // Bremsen
    v = std::clamp(v, -v_max, v_max); // Höchstgeschw. nicht überschreiten
}

float Motor::get_bremsweg() const {
    return Physik::bremsweg(v, a);
}
