#pragma once

#include "objekt.hpp"
#include "motor.hpp"

#include <cereal/types/base_class.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/optional.hpp>

/// Bewegliches Objekt.
class Objekt_Steuerbar : public Objekt {

public:

    Objekt_Steuerbar() = default;

    Objekt_Steuerbar(const Vektor& pos,
                     const Motor& motor_linear,
                     const Motor& motor_rot,
                     const Motor& motor_tauch,
                     float noise = 0.0f);

    /// Liefert den genauen Typen zur Identifikation nach Vererbung.
    Typ get_typ() const override { return Typ::OBJEKT_STEUERBAR; }

    /// Simulationstick in Sekunden.
    bool tick(Welt* welt, float s) override;

    /// Startet den Bremsvorgang, leitet kompletten Stilltstand ein.
    void stop();

    /// Zielgeschwindigkeit als Faktor der Höchstgeschwindigkeit von -1.0 bis +1.0.
    void set_target_v(float v) { motor_linear.v_target = v * motor_linear.v_max; }

    void set_target_pitch(float v) { motor_tauch.v_target = v; }

    void set_target_rudder(float v) { motor_rot.v_target = v; }

    void set_target_pos(double x, double y) { target_pos = {x,y}; }

    void set_target_bearing(float degree) { target_bearing = degree; }

    /// TODO
    void set_target_depth(dist_t depth) { target_depth = depth; }

    /// Getter: Aktuelle x/y-Geschwindigkeit (absolut).
    float get_speed() const override { return motor_linear.v; }

    /// Liefert die relative x/y-Geschwindigkeit zur Höchstgeschwindigkeit (negativ, wenn Rückwärts).
    float get_speed_relativ() const { return motor_linear.v / motor_linear.v_max; }

    /// Getter: Maximale x/y-Geschwindigkeit (absolut).
    float get_speed_max() const { return motor_linear.v_max; }

    /// Liefert die gewünschte absolute x/y-Geschwindigkeit.
    float get_target_speed() const { return motor_linear.v_target; }

    /// Liefert den Gewünschten Kurs (0-360).
    winkel_t get_target_bearing() const { return target_bearing.value_or(bearing); }

    /// Liefert die gewünschte Tiefe. @note Tiefenangaben sind negativ.
    dist_t get_target_depth()   const { return target_depth.value_or(pos.z()); }

    /// Liefert den Lärmfaktor [0.0, 1.0].
    float get_noise() const override { return noise; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt>(this),
            motor_linear, motor_rot, motor_tauch,
            target_pos, target_bearing, target_depth,
            noise
        );
    }

protected:

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
    std::optional<winkel_t> target_bearing;

    /// Zieltiefe
    std::optional<winkel_t> target_depth;

    /// Zielposition. [Aktiv,X,Y]
    std::optional<std::array<dist_t, 2>> target_pos;

    /// Lärmfaktor [0.0,1.0]
    float noise;

};
