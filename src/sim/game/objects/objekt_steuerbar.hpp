#pragma once

#include "objekt.hpp"
#include "motor.hpp"

#include <cereal/types/tuple.hpp>
#include <cereal/types/base_class.hpp>

/// Bewegliches Objekt.
class Objekt_Steuerbar : public Objekt {

public:

    Objekt_Steuerbar() = default;

    Objekt_Steuerbar(const Vektor& pos,
                     const Motor& motor_linear,
                     const Motor& motor_rot,
                     const Motor& motor_tauch);

    /// Liefert den genauen Typen zur Identifikation nach Vererbung.
    Typ get_typ() const override { return Typ::OBJEKT_STEUERBAR; }

    /// Simulationstick in Sekunden.
    bool tick(Welt* welt, float s) override;

    /// Startet den Bremsvorgang, leitet kompletten Stilltstand ein.
    void stop();

    /// Zielgeschwindigkeit als Faktor der Höchstgeschwindigkeit von -1.0 bis +1.0.
    void set_target_v(float v);

    void set_target_pitch(float degree) { motor_tauch.v_target = degree; }

    void set_target_rudder(float degree) { motor_rot.v_target = degree; }

    void set_target_pos(double x, double y);

    void set_target_bearing(float degree);

    /// TODO
    void set_target_depth(float depth);

    /// Getter: Aktuelle x/y-Geschwindigkeit (absolut).
    float get_speed() const { return motor_linear.v; }

    /// Liefert die relative x/y-Geschwindigkeit zur Höchstgeschwindigkeit (negativ, wenn Rückwärts).
    float get_speed_relativ() const { return motor_linear.v / motor_linear.v_max; }

    /// Liefert die gewünschte absolute x/y-Geschwindigkeit.
    float get_target_speed()   const { return motor_linear.v_target; }
    /// Liefert den Gewünschten Kurs (0-360).
    float get_target_bearing() const { return std::get<bool>(target_bearing) ? std::get<float>(target_bearing) : bearing; }
    /// Liefert die gewünschte Tiefe. @note Tiefenangaben sind negativ.
    float get_target_depth()   const { return std::get<bool>(target_depth) ? std::get<float>(target_depth) : pos.z(); }

    /// Liefert einen Bewegungsfaktor: 0 (steht still) bis 10 (alle Motoren auf Maximum).
    float get_noise() const override;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
            motor_linear, motor_rot, motor_tauch,
            target_pos, target_bearing, target_depth
        );
    }

private:

    /// Ruder in Zielrichtung einstellen.
    void auto_rudder();

    /// Lenken zum Zielpfad (nicht beschleunigen jedoch).
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

    /// Zielposition. [Aktiv,X,Y]
    std::tuple<bool, double, double> target_pos;

    /// Zieltiefe
    std::tuple<bool, float> target_depth;

};
