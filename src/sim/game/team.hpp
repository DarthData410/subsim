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

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(id, basis, punkte, sub_designs);
    }

private:

    /// Einmalige ID.
    uint8_t id;

    /// Position.
    Vektor basis;

    /// Punkte.
    uint32_t punkte;

    /// Verfügbare Uboote des Teams.
    std::vector<Sub> sub_designs;

};
