#pragma once

#include "sonar.hpp"

class Sonar_Aktiv final : public Sonar {

public:

    /// Nur für Serialisierung.
    Sonar_Aktiv() = default;

    Sonar_Aktiv(float noise_threshold, float resolution, const std::vector<std::tuple<float, float>>& blindspots,
                float ping_intervall_min);

    /// Führt erkennungen durch.
    void tick(Objekt* parent, Welt* welt, float s) override {}

    /// Setter: Ping Intervall.
    void set_ping_intervall(float ping_intervall) { this->detection_intervall = std::max(ping_intervall, ping_intervall_min); }

    /// Aktivieren / Deaktivieren.
    void set_aktiv(bool aktiv) { Sonar_Aktiv::aktiv = aktiv; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sonar>(this),
                aktiv, ping_intervall_min);
    }

private:

    /// Eingeschaltet?
    bool aktiv;

    /// Schnellstmögliches Intervall für Pings.
    float ping_intervall_min;

};
CEREAL_REGISTER_TYPE(Sonar_Aktiv)
