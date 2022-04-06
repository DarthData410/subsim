#pragma once

#include "../../vektor.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>

class Welt;
class Explosion;

/// Unbewegliches Objekt. Elternklasse für alle von der Physiksimulation betroffenen Objekte.
class Objekt {

    friend class Welt;
    friend class Team;

public:

    /// Liefert eine polymorphe Kopie von o via `new`.
    static Objekt* copy(const Objekt* o);

public:

    /// Vererbungshierachie von Objekt. Zur Typenbestimmung bei Laufzeit.
    enum class Typ : uint8_t {
            OBJEKT           = 0,
            OBJEKT_STEUERBAR = 1,
            SUB              = 2, // Spieler Sub
            SUB_AI           = 3, // Computergesteuertes Sub
            TORPEDO          = 4,
            EXPLOSION        = 5,
    };

    /// Ctor. Weist keine ID zu.
    Objekt() = default;

    /// Ctor. Weist neue ID zu.
    explicit Objekt(const Vektor& pos, const float& bearing = 0);

    /// Dtor.
    virtual ~Objekt() = default;

    /**
     * Führt einen Simulationstick für Zeit `s` aus.
     * @note Man kann 'false' zurückgeben, um das Objekt zu zerstören.
     *       Zerstören = wird aus der Welt entfernt.
     */
    virtual bool tick(Welt* welt, float s) {};

    /// Fügt diesem Objekt Explosionsschaden zu. Liefert `true`, wenn es durch die Explosion zerstört wurde.
    virtual bool apply_damage(Explosion* explosion, float damage) { return false; }

    /// Liefert den Objekttypen zur Polymorphieauflösung.
    virtual Typ get_typ() const { return Typ::OBJEKT; };

    /// Liefert die Geschwindigkeit des Objekts in m/s.
    virtual float get_speed() const { return 0.f; }

    /// Getter: Aktuelle Geräuschentwicklung [0.0 bis 1.0].
    virtual float get_noise() const = 0;

    /// Getter: Einmalige ID, global gültig für alle Objekte und vererbte Klassen.
    uint32_t get_id() const { return id; }

    /// Getter: Teamzugehörigkeit. 0 = Kein Team.
    uint8_t get_team() const { return team; }

    /// Getter: Position.
    const Vektor& get_pos() const { return pos; }

    /// Getter: 0° bis 360° Ausrichtung.
    float get_bearing() const { return kurs; }

    /// Getter: -90° bis 90° Steigung.
    float get_pitch() const { return pitch; }

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(id, team, pos, kurs, pitch);
    }

protected:

    /// Weist dem Objekt eine neue ID zu.
    void regenerate_id();

protected:

    /// Einmalige ID. Wird nicht aufsteigend generiert, sondern zufällig.
    oid_t id;

    /// Teamzugehörigkeit. 0 = Kein Team.
    uint8_t team = 0;

    /// x-y-Position und z-Höhe (+) bzw. z-Tiefe (-).
    Vektor pos;

    /// x/y Richtung.
    float kurs;

    /// + nach oben / - nach unten
    float pitch;

    ///
    //float groesse; // TODO idee: Wird von Distanz bei Treffern abgezogen - quasi "Objektgröße"

};
