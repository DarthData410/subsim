#pragma once

#include "objekt.hpp"
#include "motor.hpp"

#include <cereal/types/tuple.hpp>
#include <cereal/types/base_class.hpp>

class Objekt_Steuerbar : public Objekt {

public:

    Objekt_Steuerbar() = default;

    Objekt_Steuerbar(const Ogre::Vector3& pos,
                     const Motor& motor_linear,
                     const Motor& motor_rot,
                     const Motor& motor_tauch);

    /// Liefert den genauen Typen zur Identifikation nach Vererbung.
    Typ get_typ() const override { return Typ::OBJEKT_STEUERBAR; }

    /// Simulationstick in Sekunden.
    void tick(Welt* welt, float s) override;

    /// Startet den Bremsvorgang, leitet kompletten Stilltstand ein.
    void stop();

    /// Zielgeschwindigkeit als Faktor der Höchstgeschwindigkeit von -1.0 bis +1.0.
    void set_target_v(float v);

    void set_target_pitch(float degree) { motor_tauch.v_target = degree; }

    void set_target_rudder(float degree) { motor_rot.v_target = degree; }

    void set_target_pos(float x, float z);

    void set_target_bearing(float degree);

    //TODO void set_target_depth(float depth);

    /// Getter: Aktuelle x/z-Geschwindigkeit (absolut).
    float get_speed() const { return motor_linear.v; }

    /// Liefert die relative x/z-Geschwindigkeit zur Höchstgeschwindigkeit (negativ, wenn Rückwärts).
    float get_speed_relativ() const { return motor_linear.v / motor_linear.v_max; }

    /// Liefert einen Wert zwischen 0 und 1, wie gut sichtbar das fremden Objekt ist.
    //float get_sichtbarkeit(const Objekt_Steuerbar* objekt);//TODO

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
            motor_linear, motor_rot, motor_tauch,
            target_bearing, target_pos
        );
    }

private:

    /// Ruder in Zielrichtung einstellen.
    void auto_rudder();

    void auto_path();

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

};
