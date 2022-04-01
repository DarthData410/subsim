#pragma once

#include "detektion.hpp"
#include "../../typedefs.hpp"
#include <cereal/types/vector.hpp>
#include <cereal/types/tuple.hpp>

/// Prädeklarationen
class Welt;
class Objekt;

/// Passiver Sonar
class Sonar_Passiv final {

public:

    /// Default Ctor.
    Sonar_Passiv() = default;

    /// Ctor.
    Sonar_Passiv(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots);

    /// Begindet sich `objekt` in `parent`s totem Winkel (blindspot)?
    bool is_in_blindspot(winkel_t kurs_relativ) const;

    /// Getter: Mindestlärmwert (0-1), den ein Objekt haben muss, um erkannt zu werden.
    float get_noise() const { return noise; }

    /// Getter: Auf wieviel Grad genau eine Richtung bestimmt werden kann.
    float get_resolution() const { return resolution; }

    /// Getter: Sichtbereich(e) des Sonars, relativ zum sub.
    const std::optional<std::vector<std::tuple<float, float>>>& get_blindspots() const { return blindspots; }

    /// Getter: Aktuelle erkannte Signaturen.
    const auto& get_detections() const { return detections; }

    /// Führt erkennungen durch.
    void tick(Objekt* parent, Welt* welt, float s);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(noise, resolution, blindspots, detections);
    }

private:

    /// Grund-Noise-Level. Eigene Sub-Geräusche kommen noch hinzu. 0.0 = Perfekt, 1.0 = Detektionen praktisch unmöglich.
    float noise;

    /// Auflösung. Auf wieviel Grad ° genau die Detektion ist.
    float resolution;

    /// Stoppuhr, vor wievielen Sekunden die letzte Detektionsberechnung durchgeführt wurde.
    float timer;

    /// Ausrichtung des Sonars relativ zum Mutterschiff.
    // uint8_t ausrichtung; TODO notwendig? Einfach immer 0

    /// Winkelbereiche (min,max), zwischen denen dieses Sonar nichts sieht.
    std::vector<std::tuple<float, float>> blindspots;

    /// Alle aktuellen Detektionen.
    std::vector<Detektion> detections;

};
