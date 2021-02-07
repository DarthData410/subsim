#pragma once

#include <cereal/types/vector.hpp>

/// Prädeklarationen
class Welt;
class Sub;

/// Einzelne Sichtung.
struct Detection {
    enum Typ : uint8_t {
        MOVEMENT_SIGNATURE = 1,
    };
    uint32_t objekt_id;
    float gain;
    float bearing;
    Typ typ;

    template <class Archive> void serialize(Archive& ar) {
        ar(objekt_id, gain, bearing, typ);
    }
};

/// Passiver Sonar
class Sonar_Passiv final {

public:

    /// Default Ctor.
    Sonar_Passiv() = default;

    /// Ctor.
    Sonar_Passiv(float noise, uint16_t resolution, uint8_t sichtbereich = 180);

    /// Führt erkennungen durch.
    void tick(Sub* parent, Welt* welt, float s);

    /// Getter: Aktuelle erkannte Signaturen.
    const auto& get_detections() const { return detections; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(noise, resolution, sichtbereich, ausrichtung, detections);
    }

private:

    /// Grund-Noise-Level. Eigene Sub-Geräusche kommen noch hinzu. 0.0 = Perfekt, 1.0 = Detektionen unmöglich.
    float noise;

    /// Auflösung. 360° = Perfekt. 180° bedeutet eine Darstellung von 1° auf 2°.
    uint16_t resolution;

    /// Sichtbereich. 90 = von 315 über 0 bis 45. 180° heißt rundum.
    uint8_t sichtbereich;

    /// Ausrichtung des Sonars relativ zum Mutterschiff.
    uint8_t ausrichtung;

    /// Alle aktuellen Detektionen.
    std::vector<Detection> detections;

};
