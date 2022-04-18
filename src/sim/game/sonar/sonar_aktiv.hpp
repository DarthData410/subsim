#pragma once

#include "sonar.hpp"

class Sonar_Aktiv final : public Sonar {

    friend class Kommando;

public:

    /// Operationsmodus des aktiven Sonars. Aus / Einzelner Ping / Eingeschaltet = wiederholender Ping.
    enum class Mode : uint8_t { OFF, SINGLE, ON };

    /// Nur für Serialisierung.
    Sonar_Aktiv() = default;

    /// Ctor. @note Aktive Sonars sind standardmäßig ausgeschaltet.
    Sonar_Aktiv(const std::string& name, Groesse groesse, float resolution, dist_t max_range, float ping_intervall_min,
                const std::vector<std::tuple<float, float>>& blindspots);

    /// Führt erkennungen durch.
    void tick(Objekt* parent, Welt* welt, float s) override;

    /// Setter: Ping Intervall. @note Nutzt `std::max` um min nicht zu unterschreiten.
    void set_ping_intervall(float ping_intervall) { this->intervall = std::max(ping_intervall, ping_intervall_min); }

    /// Aktivieren / Deaktivieren.
    void set_mode(Mode mode) { Sonar_Aktiv::mode = mode; }

    /// Getter: Aktueller Operationsmodus.
    Mode get_mode() const { return mode; }

    /// Getter: Schnellstmöglicher Ping-Intervall.
    float get_ping_intervall_min() const { return ping_intervall_min; }

    /// Getter: Maximale Reichweite in m.
    dist_t get_max_range() const { return max_range; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sonar>(this),
                mode, ping_intervall_min, max_range);
    }

private:

    /// Eingeschaltet?
    Mode mode;

    /// Erkennungsreichweite der Pings.
    dist_t max_range;

    /// Schnellstmögliches Intervall für Pings.
    float ping_intervall_min;

};
CEREAL_REGISTER_TYPE(Sonar_Aktiv)
