#pragma once

#include <cereal/types/tuple.hpp>

class Welt;

class Zone final {

public:

    /// Ctor default.
    Zone() = default;

    Zone(const std::tuple<float, float>& pos, float groesse);

    /// Getter: Das kontrollierende Team. 0, wenn niemand.
    uint8_t get_team() const { return team; }

    /// Getter: Position X,Z.
    const std::tuple<float, float>& get_pos() const { return pos; }

    /// Getter: Groesse.
    float get_groesse() const { return groesse; }

    /// Einzelner Simulationsschritt; Zeit in Sekunden.
    void tick(Welt* welt, float s);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(team, pos, groesse);
    }

private:

    uint8_t team = 0;

    std::tuple<float, float> pos;

    float groesse;

};