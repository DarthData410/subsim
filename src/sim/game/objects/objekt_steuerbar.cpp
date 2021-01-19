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

void Objekt_Steuerbar::tick(float s) {
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

void Objekt_Steuerbar::auto_rudder() {
    const float target_yaw = std::get<float>(target_bearing) - 180.f;
    const float rotate_to = target_yaw - orientation.getYaw().valueDegrees();
    if (std::abs(rotate_to) < motor_rot.v_max) {
        Physik::rotate(orientation, motor_rot.v);
        std::get<bool>(target_bearing) = false;
        motor_rot.v_target = 0;
    }
    else if (rotate_to < 0) motor_rot.v_target = -motor_rot.v_max;
    else if (rotate_to > 0) motor_rot.v_target =  motor_rot.v_max;
}
