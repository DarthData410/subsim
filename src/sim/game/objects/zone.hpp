#pragma once

#include <cereal/types/tuple.hpp>

class Welt;

/// Eine Zone in der Welt, die von einem Team besetzt werden kann, um Punkte zu bekommen.
class Zone final {

public:

    /// Ctor default.
    Zone() = default;

    Zone(const std::tuple<float, float>& pos, float groesse);

    /// Getter: Das kontrollierende Team. 0, wenn niemand.
    uint8_t get_team() const { return team; }

    /// Getter: Position X,Z; Standort dieser Zone (Zentrum).
    const std::tuple<float, float>& get_pos() const { return pos; }

    /// Getter: Größe bzw. Radius dieser Zone um gg. Position.
    float get_groesse() const { return groesse; }

    /// Einzelner Simulationsschritt; Zeit in Sekunden.
    void tick(Welt* welt, float s);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(team, pos, groesse);
    }

private:

    /// Besetzt von Team `team`.
    uint8_t team = 0;

    /// Standort dieser Zone (Zentrum).
    std::tuple<float, float> pos;

    /// Größe bzw. Radius dieser Zone um gg. Position.
    float groesse;

};
