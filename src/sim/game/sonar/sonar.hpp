#pragma once

#include "detektion.hpp"
#include "../../typedefs.hpp"

#include <cereal/types/vector.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/polymorphic.hpp>

/// Prädeklarationen
class Welt;
class Objekt;

class Sonar {

public:

    /// Nur für die Serialisierung.
    Sonar() = default;

    Sonar(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots);

    /// Führt erkennungen durch.
    virtual void tick(Objekt* parent, Welt* welt, float s) = 0;

    /// Begindet sich `objekt` in `parent`s totem Winkel (blindspot)?
    bool is_in_toter_winkel(winkel_t kurs_relativ) const;

    /// Getter: Mindestlärmwert (0-1), den ein Objekt haben muss, um erkannt zu werden.
    float get_empfindlichkeit() const { return noise; }

    /// Getter: Auf wieviel Grad genau eine Richtung bestimmt werden kann.
    float get_aufloesung() const { return resolution; }

    /// Getter: Sichtbereich(e) des Sonars, relativ zum sub.
    const std::optional<std::vector<std::tuple<float, float>>>& get_blindspots() const { return blindspots; }

    /// Getter: Aktuelle erkannte Signaturen.
    const auto& get_detektionen() const { return detektionen; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(noise, resolution, timer, detection_intervall, blindspots, detektionen);
    }

protected:

    /**
     * Grund-Noise-Level. Eigene Sub-Geräusche kommen noch hinzu. 0.0 = Perfekt, 1.0 = Detektionen praktisch unmöglich.
     * Ist praktisch auch die Reichweite des Sonars.
     */
    float noise;

    /// Auflösung. Auf wieviel Grad ° genau die Detektion ist.
    float resolution;

    /// Stoppuhr bis zur nächsten Erkennungsberechnung (Passiv) / bis zum nächsten Ping (Aktiv).
    float timer;

    /// Alle x Sekunden wird der Detektionsalgorithmus ausgeführt (Passiv) / ein Ping abgegeben (Aktiv).
    float detection_intervall = 1;

    /// Winkelbereiche (min,max), zwischen denen dieses Sonar nichts sieht.
    std::vector<std::tuple<float, float>> blindspots;

    /// Alle aktuellen Detektionen (des letzten Ticks). Historie muss anderswo gespeichert / verarbeitet werden.
    std::vector<Detektion> detektionen;

};
CEREAL_REGISTER_TYPE(Sonar)
