#pragma once

#include "objects/sub.hpp"
#include <vector>

class Team final {

    friend class Welt;
    friend class Zone;

public:

    Team() = default;

    /// Liefert ein neues Team mit gegebener ID.
    explicit Team(uint8_t id);

    /// Gibt ein Design für ein neues Sub.
    const Sub& get_new_sub();

    /// Getter: Position des Hauptquartierts.
    const std::tuple<float, float>& get_pos() const { return basis; };

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(id, basis, punkte, sub_designs);
    }

private:

    /// Einmalige ID.
    uint8_t id;

    /// Verfügbare Uboote des Teams.
    std::vector<Sub> sub_designs;

    /// Position.
    std::tuple<float, float> basis;

    /// Punkte.
    unsigned punkte;

};
