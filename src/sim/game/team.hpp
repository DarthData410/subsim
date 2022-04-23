#pragma once

#include "objekte/sub.hpp"

/// Ein Team, dem Spieler angehören, das Punkte hat und neue U-Boote erforscht usw.
class Team final {

    friend class Welt;
    friend class Zone;

public:

    /// Ctor.
    Team() = default;

    /// Liefert ein neues Team mit gegebener ID.
    explicit Team(uint8_t id);

    /// Getter: Position des Hauptquartierts.
    const Vektor& get_basis() const { return basis; };

    /// Getter: Punkte.
    uint32_t get_punkte() const { return punkte; }

    /// Gibt ein Design für ein neues Sub.
    const Sub& get_new_sub();

    /// Gibt die aktuellen Subdesigns des Teams.
    const std::vector<Sub>& get_sub_designs() const { return sub_designs; }

    /// Liefert die aktuellen Torpedodesigns des Teams.
    const std::vector<Torpedo>& get_torp_designs() const { return torp_designs; }

    /// Liefert die aktuellen Aktiven Sonar Designs des Teams.
    const std::vector<Sonar_Aktiv>& get_sonars_a() const { return sonars_a; }

    /// Liefert die aktuellen Passiven Sonar Designs des Teams.
    const std::vector<Sonar_Passiv>& get_sonars_p() const { return sonars_p; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(id, basis, punkte, sub_designs, torp_designs, sonars_a, sonars_p);
    }

private:

    /// Einmalige ID.
    uint8_t id;

    /// Position.
    Vektor basis;

    /// Punkte.
    uint32_t punkte;

    /// Verfügbare U-Boottypen des Teams und deren Sonars.
    std::vector<Sub> sub_designs;

    /// Verfügbare Torpedotypen des Teams und deren Sonars.
    std::vector<Torpedo> torp_designs;

    /// Verfügbare Aktive Sonars des Teams.
    std::vector<Sonar_Aktiv> sonars_a;

    /// Verfügbare Passive Sonars des Teams.
    std::vector<Sonar_Passiv> sonars_p;

};
