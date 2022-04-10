#pragma once

#include "objekt_steuerbar.hpp"
#include "../sonar/sonar_passiv.hpp"
#include "torpedo.hpp"
#include "../sonar/sonar_aktiv.hpp"

#include <cereal/types/map.hpp>

/// Ein U-Boot.
class Sub : public Objekt_Steuerbar {

    friend class Team;
    friend class Test_Physik;

public:

    Sub() = default;

    Sub(const Vektor& pos,
        const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch);

    bool tick(Welt* welt, float s) override;

    Typ get_typ() const override { return Typ::SUB; }

    /// Torpedo mit gg. Namen (sofort) abschießen. `false`, wenn nicht möglich, weil z.B. keine Munition.
    bool shoot(const std::string& torpedo_name);

    /// Liefert alle aktiven Sonars an Board. Kann leer sein.
    const std::vector<Sonar_Aktiv>&  get_sonars_active()  const { return sonars_active; }
    /// Liefert alle passiven Sonars an Board. Kann leer sein.
    const std::vector<Sonar_Passiv>& get_sonars_passive() const { return sonars_passive; }

    /// Liefert alle Torpedotypen an Board und deren verfügbare Anzahl.
    const std::map<Torpedo, uint8_t>& get_torpedos() const { return torpedos; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Objekt_Steuerbar>(this),
           sonars_active, sonars_passive, torpedos
        );
    }

protected:

    /// Aktive Sonars an Board.
    std::vector<Sonar_Aktiv> sonars_active;

    /// Passive Sonars an Board.
    std::vector<Sonar_Passiv> sonars_passive;

    /// Torpedos und deren Anzahl.
    std::map<Torpedo, uint8_t> torpedos;

};
CEREAL_REGISTER_TYPE(Sub)
