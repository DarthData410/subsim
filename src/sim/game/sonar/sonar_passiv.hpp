#pragma once

#include "sonar.hpp"

/// Passiver Sonar
class Sonar_Passiv final : public Sonar {

public:

    /// Default Ctor.
    Sonar_Passiv() = default;

    /// Ctor.
    Sonar_Passiv(Groesse groesse, float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots);

    /// Führt erkennungen durch.
    void tick(Objekt* parent, Welt* welt, float s) override;

    /// Getter: Mindestlärmwert (0-1), den ein Objekt haben muss, um erkannt zu werden.
    float get_empfindlichkeit() const { return noise; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sonar>(this),
            noise);
    }

private:

    /**
     * Grund-Noise-Level. Eigene Sub-Geräusche kommen noch hinzu.
     * 0.0 = Perfekt, 1.0 = Detektionen praktisch unmöglich.
     * Ist praktisch auch die Reichweite des Sonars.
     */
    float noise;

};
CEREAL_REGISTER_TYPE(Sonar_Passiv)
