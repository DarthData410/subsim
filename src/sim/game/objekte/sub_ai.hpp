#pragma once

#include "sub.hpp"

#include <SFML/System/Clock.hpp>

/// Ein Computergesteuertes U-B
class Sub_AI final : public Sub {

    friend class Test_Bots;

    static constexpr float SPEED_TRAVEL = 0.75;
    static constexpr float DEPTH_TRAVEL = -50;
    static constexpr float TARGET_DISTANCE = 1000;
    static constexpr float AI_THINK_INTERVALL = 10; // Sekunden

    enum Status : uint8_t { // nicht vergessen: größeren Typen bei wachsender Zahl Status
        DONE    = 0,        // Keine Aufgabe (mehr).
        TRAVEL  = 1 << 0,   // Reisen zu bestimmter Position.
        SEARCH  = 1 << 1,   // Nach Feinden suchen, auch mit AS.
        HIDE    = 1 << 2,   // Verstecken vor Feinden.
        //EVADE   = 1 << 3,   // Torpedos / Waffen ausweichen. TODO
        //ATTACK  = 1 << 4,   // Feinde Angreifen.
        //WAIT    = 1 << 5,   // absolut nichts tun
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
        ar(cereal:: base_class<Sub>(this));
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

    /// Liefert die Entfernung zum Ziel in m.
    dist_t get_zielentfernung() const;

    /// Weckt die KI und lässt sie sofort eine neue Aufgabe finden.
    void wake() { timer_next_action = 0; }

    /// Aktualisiert `ziele_torpedos`.
    void update_ziele_torpedos(const Welt* welt);

    /// Liefert aus den Sonarerkennungen Gegner, die als Ziel für dieses AI-Sub dienen könnten.
    void get_valide_ziele(const Welt* welt, std::vector<const Detektion*>& d_active, std::vector<const Detektion*>& d_passive);

    /// Schritt für Schritt auf Gegner zubewegen, wenn in Reichweite auch Torpedo feuern.
    void maybe_attack(Welt* welt,
                      const std::vector<const Detektion*>& detektionen,
                      const std::vector<const Detektion*>& d_active,
                      const std::vector<const Detektion*>& d_passive);

    /// Wählt je nach Situation neue Aufgaben aus für `status`.
    void chose_new_job(const Welt* welt);

private:

    /// Timer für neue Aufgaben.
    float timer;

    /// Wecker, wann die nächste Entscheidung getroffen werden soll.
    float timer_next_action;

    /// Zielkoordinaten. Zieltyp ist kontextabhängig.
    Vektor ziel;

    /// Aktuelle Aufgabe.
    uint8_t status = DONE;

    /// Speichert, welche Objekte (IDs) mit welchen Torpedos (IDs) aktuell unter Beschuss sind.
    std::unordered_map<oid_t, std::unordered_set<oid_t>> ziele_torpedos;

};
CEREAL_REGISTER_TYPE(Sub_AI)
