#pragma once

#include "objekt.hpp"
#include "motor.hpp"

class Objekt_Steuerbar : public Objekt {

public:

    Objekt_Steuerbar() = default;

    Objekt_Steuerbar(const Ogre::Vector3& pos,
                     const Motor& motor_linear,
                     const Motor& motor_rot,
                     const Motor& motor_tauch);

    /// Liefert den genauen Typen zur Identifikation nach Vererbung.
    virtual Typ get_typ() const override { return Typ::OBJEKT_STEUERBAR; }

    /// Simulationstick in Sekunden.
    virtual void tick(Welt* welt, float s) override;

    /// Startet den Bremsvorgang, leitet kompletten Stilltstand ein.
    void stop();

    void set_target_v(float v) { motor_linear.v_target = v; }

    void set_target_pitch(float degree) { motor_tauch.v_target = degree; }

    void set_target_rudder(float degree) { motor_rot.v_target = degree; }

    void set_target_pos(float x, float z);

    void set_target_bearing(float degree);

    //TODO void set_target_depth(float depth);

protected:

    /// Motor zur linearen Bewegung.
    Motor motor_linear;

    /// Motor zur links/rechts - Bewegung.
    Motor motor_rot;

    /// Motor zum Auf/Abtauchen.
    Motor motor_tauch;

    /// Zielrichtung. [Aktiv,Grad]
    std::tuple<bool, float> target_bearing;

    /// Zielposition. [Aktiv,X,Z]
    std::tuple<bool, float, float> target_pos;

    /// Zieltiefe
    // TODO std::tuple<bool, float> target_depth;

private:

    /// Ruder in Zielrichtung einstellen.
    void auto_rudder();

};
