#include "objekt_steuerbar.hpp"
#include "../../physik.hpp"

Objekt_Steuerbar::Objekt_Steuerbar(const Ogre::Vector3& pos,
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

void Objekt_Steuerbar::tick(Welt* welt, float s) {
    static constexpr float eps = 0.001f;

    // Automatische ausrichtung Links/Rechts
    if (std::get<bool>(target_bearing)) auto_rudder();

    // Beschleunigen/Bremsen
    motor_linear.tick(s);
    motor_tauch.tick(s);
    motor_rot.tick(s);

    // Auf/Ab in Grad festlegen
    //Physik::set_pitch(orientation, motor_tauch.v - 90.f); // TODO
    pos.y += motor_tauch.v; // TODO workaround für Quaternion pitch

    // Links/Rechts in Grad bewegen
    if (std::abs(motor_rot.v) > eps) Physik::rotate(orientation, motor_rot.v * s);

    // Vorwärts/Rückwärts in m bewegen
    if (std::abs(motor_linear.v) > eps) Physik::move_ahead(pos, orientation, motor_linear.v * s);
}

void Objekt_Steuerbar::set_target_bearing(float degree) {
    std::get<bool> (target_bearing) = true;
    std::get<float>(target_bearing) = degree;
}

void Objekt_Steuerbar::set_target_pos(float x, float z) {
    std::get<bool>(target_pos) = true;
    std::get<1>(target_pos) = x;
    std::get<2>(target_pos) = z;
}

void Objekt_Steuerbar::auto_rudder() {
    float rotate_to = std::get<float>(target_bearing) - get_bearing();
    if (rotate_to > 180.f) rotate_to = std::get<float>(target_bearing) - 360 + get_bearing();
    if (std::abs(rotate_to) <= (motor_rot.v * motor_rot.v) / (2.0f * motor_rot.a)) { // Erledigt -> Bremsen
        motor_rot.v_target = 0;
        std::get<bool>(target_bearing) = false;
    }
    else if (rotate_to < 0) motor_rot.v_target = -motor_rot.v_max;
    else if (rotate_to > 0) motor_rot.v_target =  motor_rot.v_max;
}
