#pragma once

#include "objekt.hpp"
#include "motor.hpp"

class Objekt_Steuerbar : public Objekt {

public:

    Objekt_Steuerbar() = default;

    Objekt_Steuerbar(const Ogre::Vector3& pos,
                     const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch);

    /// Simulationstick in Sekunden.
    void tick(float s);

    void stop() {
        motor_linear.v_target = 0;
        motor_rot.v_target    = 0;
        motor_tauch.v_target  = 0;
    }

    void set_target_v(float v) { motor_linear.v_target = v; }

    void set_target_pitch(float degree) { motor_tauch.v_target = degree; }

    void set_target_rudder(float degree) { motor_rot.v_target = degree; }

    void set_target_bearing(float degree);

protected:

    /// Motor zur linearen Bewegung.
    Motor motor_linear;

    /// Motor zur links/rechts - Bewegung.
    Motor motor_rot;

    /// Motor zum Auf/Abtauchen.
    Motor motor_tauch;

    /// Zielrichtung. [Aktiv,Grad]
    std::tuple<bool, float> target_bearing;

    /// Zieltiefe
    // TODO

private:

    /// Ruder in Zielrichtung einstellen.
    void auto_rudder();

};
