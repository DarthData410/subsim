#pragma once

#include "objekt_steuerbar.hpp"
#include "../sonar/sonar_aktiv.hpp"
#include "../sonar/sonar_passiv.hpp"
#include "torpedo.hpp"
#include "decoy.hpp"

#include <cereal/types/map.hpp>

/// Ein U-Boot.
class Sub : public Objekt_Steuerbar {

    friend class Team;
    friend class Kommando;
    friend class Test_Physik;
    friend class Test_Welt;
    friend class Test_Objekte;

public:

    /// Mindesttiefe für U-Boote.
    static constexpr float SURFACE_DEPTH = -5.f;

    Sub() = default;

    Sub(const std::string& name, const Vektor& pos,
        const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch);

    bool tick(Welt* welt, float s) override;

    Typ get_typ() const override { return Typ::SUB; }

    const std::string& get_name() const override { return name; }

    /// Waffe mit gg. Namen (sofort) abschießen. `false`, wenn nicht möglich, weil z.B. keine Munition.
    bool shoot(const std::string& name, Typ torpedo_oder_gegenmassnahme);

    /// Liefert alle aktiven Sonars an Board. Kann leer sein.
    const std::vector<Sonar_Aktiv>&  get_sonars_active()  const { return sonars_active; }

    /// Liefert alle passiven Sonars an Board. Kann leer sein.
    const std::vector<Sonar_Passiv>& get_sonars_passive() const { return sonars_passive; }

    /// Liefert alle Torpedotypen an Board und deren verfügbare Anzahl.
    const std::map<Torpedo, uint8_t>& get_torpedos() const { return torpedos; }

    /// Liefert die höchste Reichweite aller AS.
    dist_t get_max_reichweite_as() const;

    /// Liefert die beste Reichweite aller Torpedos (auch derer ohne Munition).
    float get_max_reichweite_torpedo() const;

    /// Liefert die beste Empfindlichkeit aller PS.
    float get_beste_empfindlichkeit_ps() const;

    /// Liefert die beste Auflösung aller Sonars (AS & PS).
    float get_best_aufloesung_sonar() const;

    /// Liefert die Tiefe (z negativ), ab der geschnorchelt werden kann.
    float get_schnorcheltiefe() const { return -50.f; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt_Steuerbar>(this),
           name, sonars_active, sonars_passive, torpedos, decoys
        );
    }

protected:

    /// Name des U-Bootmodells.
    std::string name;

    /// Aktive Sonars an Board.
    std::vector<Sonar_Aktiv> sonars_active;

    /// Passive Sonars an Board.
    std::vector<Sonar_Passiv> sonars_passive;

    /// Torpedos und deren Anzahl.
    std::map<Torpedo, uint8_t> torpedos;

    /// Decoys und deren Anzahl.
    std::map<Decoy, uint8_t> decoys;

};
CEREAL_REGISTER_TYPE(Sub)
