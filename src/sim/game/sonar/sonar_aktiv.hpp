#pragma once

#include "sonar.hpp"

class Sonar_Aktiv final : public Sonar {

public:

    /// Führt erkennungen durch.
    void tick(Objekt* parent, Welt* welt, float s) override {}

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sonar>(this));
    }

};
CEREAL_REGISTER_TYPE(Sonar_Aktiv)
