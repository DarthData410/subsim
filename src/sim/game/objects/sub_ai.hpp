#pragma once

#include <SFML/System/Clock.hpp>
#include "sub.hpp"

/// Ein Computergesteuertes U-B
class Sub_AI final : public Sub {

    enum Status : uint8_t {
        DONE    = 0,        // Keine Aufgabe (mehr).
        TRAVEL = 1 << 0,    // Reisen zu bestimmter Position.    
        SEARCH  = 1 << 1,   // Nach Feinden suchen.
        HIDE    = 1 << 2,   // Verstecken vor Feinden.
        EVADE   = 1 << 3,   // Torpedos / Waffen ausweichen.
        ATTACK  = 1 << 4,   // Feinde Angreifen.
    };

public:

    /// Ctor.
    Sub_AI() = default;

    /// Erstellt ein Computergesteuertes U-Boot aus gegebener Vorlage.
    explicit Sub_AI(const Sub& sub);

    Typ get_typ() const override { return Typ::SUB_AI; }

    bool tick(Welt* welt, float s) override;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(cereal::base_class<Sub>(this));
    }

private:

    /// Ist gg. status gesetzt?
    bool hat_status(Status status) const { return this->status & status; }

    /// Status löschen; auf fertig zurücksetzen.
    void clear_status() { status = DONE; }

    /// Fügt gg. Status hinzu.
    void add_status(Status status) { this->status |= status; }

    /// Entfernt gg. Status. @note `DONE` darf so nicht entfernt werden, stattdessen: `clear_status()`.
    void remove_status(Status status) { this->status &= ~status; }

private:

    /// Timer für neue Aufgaben.
    sf::Clock timer;

    /// Zielkoordinaten. Zieltyp ist kontextabhängig.
    Vektor ziel;

    /// Aktuelle Aufgabe.
    uint8_t status = DONE;

};
