#pragma once

#include "sonar.hpp"

/// Passiver Sonar
class Sonar_Passiv final : public Sonar {

public:

    /// Default Ctor.
    Sonar_Passiv() = default;

    /// Ctor.
    Sonar_Passiv(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots);

    /// Führt erkennungen durch.
    void tick(Objekt* parent, Welt* welt, float s) override;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sonar>(this));
    }

};
CEREAL_REGISTER_TYPE(Sonar_Passiv)
