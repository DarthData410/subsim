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

    /// Sonar-Größe: Klein nur für Torpedos; Groß für Subs
    enum class Groesse : uint8_t {
        KLEIN,
        GROSS,
    };

    /// Nur für die Serialisierung.
    Sonar() = default;

    Sonar(const std::string& name, Groesse groesse, float resolution, std::vector<std::tuple<float, float>> blindspots);

    /// Führt erkennungen durch.
    virtual void tick(Objekt* parent, Welt* welt, float s) = 0;

    /// Begindet sich `objekt` in `parent`s totem Winkel (blindspot)?
    bool is_in_toter_winkel(winkel_t kurs_relativ) const;

    /// Getter: Auf wieviel Grad genau eine Richtung bestimmt werden kann.
    float get_aufloesung() const { return resolution; }

    const std::string& get_name() const { return name; }

    float get_resolution() const { return resolution; }

    Groesse get_groesse() const { return groesse; }

    /// Getter: Sichtbereich(e) des Sonars, relativ zum sub.
    const std::vector<std::tuple<float, float>>& get_blindspots() const { return blindspots; }

    /// Getter: Aktuelle erkannte Signaturen.
    const auto& get_detektionen() const { return detektionen; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(name, resolution, timer, intervall, blindspots, detektionen, groesse);
    }

protected:

    /// Name des Sonars.
    std::string name;

    /// Auflösung. Auf wieviel Grad ° genau die Detektion ist.
    float resolution;

    /// Stoppuhr bis zur nächsten Erkennungsberechnung (Passiv) / bis zum nächsten Ping (Aktiv).
    float timer;

    /// Alle x Sekunden wird der Detektionsalgorithmus ausgeführt (Passiv) / ein Ping abgegeben (Aktiv).
    float intervall = 1;

    /// Passt der Sonar nur in Torpedos oder nur in Subs?
    Groesse groesse;

    /// Winkelbereiche (min,max), zwischen denen dieses Sonar nichts sieht.
    std::vector<std::tuple<float, float>> blindspots;

    /// Alle aktuellen Detektionen (des letzten Ticks). Historie muss anderswo gespeichert / verarbeitet werden.
    std::vector<Detektion> detektionen;

};
CEREAL_REGISTER_TYPE(Sonar)
