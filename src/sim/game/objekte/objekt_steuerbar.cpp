#include <log.hpp>
#include <valarray>
#include "objekt_steuerbar.hpp"
#include "../../physik.hpp"

Objekt_Steuerbar::Objekt_Steuerbar(const Vektor& pos,
                                   const Motor& motor_linear,
                                   const Motor& motor_rot,
                                   const Motor& motor_tauch,
                                   float noise)
        : Objekt(pos),
          motor_linear(motor_linear), motor_rot(motor_rot), motor_tauch(motor_tauch),
          noise(noise)
{
    //
}

void Objekt_Steuerbar::stop() {
    motor_linear.v_target = 0;
    motor_rot.v_target    = 0;
    motor_tauch.v_target  = 0;
    target_bearing = std::nullopt;
    target_pos     = std::nullopt;
    target_depth   = std::nullopt;
}

bool Objekt_Steuerbar::tick(Welt* welt, float s) {
    static constexpr float eps = 0.0001f;

    // Automatisches Pfadfinden
    if (target_pos) auto_path();

    // Automatische ausrichtung Links/Rechts
    if (target_bearing) auto_rudder();

    // Beschleunigen/Bremsen
    motor_linear.tick(s);
    motor_tauch.tick(s);
    motor_rot.tick(s);

    // Links/Rechts in Grad bewegen
    // + Max. Rotation ist Geschwindigkeitsabhängig
    const float max_rot = std::sqrt(std::abs(get_speed_relativ()));
    if (std::abs(motor_rot.v) > motor_rot.v_max * max_rot) motor_rot.v = motor_rot.v * max_rot;
    if (std::abs(motor_rot.v) > eps) bearing += motor_rot.v * s;

    // Vorwärts/Rückwärts in m bewegen
    if (std::abs(motor_linear.v) > eps) Physik::move(pos, bearing, motor_linear.v * s);

    // TODO target_depth
    return true;
}

void Objekt_Steuerbar::set_target_bearing(float degree) {
    target_bearing = degree;
}

void Objekt_Steuerbar::set_target_pos(double x, double y) {
    target_pos = {x,y};
}

void Objekt_Steuerbar::auto_rudder() {
    const float rotate_to = Physik::winkel_diff(get_bearing(), target_bearing.value());
    if (std::abs(rotate_to) <= motor_rot.get_bremsweg()) { // Erledigt -> Bremsen
        motor_rot.v_target = 0;
        target_bearing = std::nullopt;
    }
    else if (rotate_to < 0) motor_rot.v_target = -motor_rot.v_max;
    else if (rotate_to > 0) motor_rot.v_target =  motor_rot.v_max;
}

void Objekt_Steuerbar::auto_path() {
    // Richtung
    const auto target_x = target_pos.value()[0];
    const auto target_y = target_pos.value()[1];
    const auto bearing = Physik::kurs(pos.x(), pos.y(), target_x, target_y);
    if (std::abs(get_bearing() - bearing) > 1.f) set_target_bearing(bearing);
    if (Physik::distanz(pos.x(), pos.y(), target_x, target_y) <= motor_linear.get_bremsweg()) {
        target_pos = std::nullopt; // Ziel erreicht.
        set_target_bearing(bearing);
        stop();
    }
}

void Objekt_Steuerbar::set_target_v(float v) {
    motor_linear.v_target = v * motor_linear.v_max;
}

void Objekt_Steuerbar::set_target_depth(float depth) {
    target_depth = depth;
}

float Objekt_Steuerbar::get_noise() const {
    return noise;
}
