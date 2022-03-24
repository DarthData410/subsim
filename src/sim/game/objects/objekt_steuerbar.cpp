#include <log.hpp>
#include <valarray>
#include "objekt_steuerbar.hpp"
#include "../../physik.hpp"

Objekt_Steuerbar::Objekt_Steuerbar(const Vektor& pos,
                                   const Motor& motor_linear,
                                   const Motor& motor_rot,
                                   const Motor& motor_tauch)
        : Objekt(pos),
          motor_linear(motor_linear), motor_rot(motor_rot), motor_tauch(motor_tauch),
          target_bearing(false, 0)
{
    //
}

void Objekt_Steuerbar::stop() {
    motor_linear.v_target = 0;
    motor_rot.v_target    = 0;
    motor_tauch.v_target  = 0;
    std::get<bool>(target_bearing) = false;
    std::get<bool>(target_pos)     = false;
    // TODO std::get<bool>(target_depth) = false;
}

bool Objekt_Steuerbar::tick(Welt* welt, float s) {
    static constexpr float eps = 0.0001f;

    // Automatisches Pfadfinden
    if (std::get<bool>(target_pos)) auto_path();

    // Automatische ausrichtung Links/Rechts
    if (std::get<bool>(target_bearing)) auto_rudder();

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
    return true;
}

void Objekt_Steuerbar::set_target_bearing(float degree) {
    std::get<bool> (target_bearing) = true;
    std::get<float>(target_bearing) = degree;
}

void Objekt_Steuerbar::set_target_pos(double x, double y) {
    std::get<bool>(target_pos) = true;
    std::get<1>(target_pos) = x;
    std::get<2>(target_pos) = y;
}

void Objekt_Steuerbar::auto_rudder() {
    const float rotate_to = Physik::winkel_diff(get_bearing(), std::get<float>(target_bearing));
    if (std::abs(rotate_to) <= motor_rot.get_bremsweg()) { // Erledigt -> Bremsen
        motor_rot.v_target = 0;
        std::get<bool>(target_bearing) = false;
    }
    else if (rotate_to < 0) motor_rot.v_target = -motor_rot.v_max;
    else if (rotate_to > 0) motor_rot.v_target =  motor_rot.v_max;
}

void Objekt_Steuerbar::auto_path() {
    // Richtung
    const float target_x = std::get<1>(target_pos);
    const float target_y = std::get<2>(target_pos);
    const float bearing = Physik::kurs(pos.x(), pos.y(), target_x, target_y);
    if (std::abs(get_bearing() - bearing) > 1.f) set_target_bearing(bearing);
    if (Physik::distanz(pos.x(), pos.y(), target_x, target_y) <= motor_linear.get_bremsweg()) {
        std::get<bool>(target_pos) = false; // Ziel erreicht.
        set_target_bearing(bearing);
        stop();
    }
}

void Objekt_Steuerbar::set_target_v(float v) {
    motor_linear.v_target = v * motor_linear.v_max;
}

float Objekt_Steuerbar::get_noise() const {
    const float noise = std::abs(motor_tauch.v)    / motor_tauch.v_max +
                        std::abs(motor_rot.v)      / motor_rot.v_max +
                        std::abs((0.5f * (motor_linear.v + motor_linear.v_target)) / motor_linear.v_max * 8.f);
    return noise;
}
