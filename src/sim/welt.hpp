#pragma once

#include "game/team.hpp"
#include "game/zone.hpp"
#include "game/abschuss.hpp"
#include "game/karte.hpp"

class Sub;
class Decoy;
class Torpedo;

/// Simluierte Welt, die alle Objekte enthält.
class Welt final {

    /// Welt hat privaten Zugriff auf folgende Klassen:
    friend class Host;
    friend class Kommando;
    friend class Zone;
    friend class Sub_AI;
    friend class Sonar_Passiv;
    friend class Torpedo;
    friend class Test_Welt;
    friend class Test_Objekte;

public:

    /// Erstellt eine nutzbare Welt. Teams, Zonen und Computergegner werden auch generiert.
    explicit Welt(unsigned npcs_pro_team = 1);

    /** 
     * Führt einen Simulationsschritt durch. Die Methode misst selbst die Zeit, wann sie davor zuletzt
     * aufgerufen wurde und skaliert entsprechend alle physikalischen Bewegungen etc.
     * */
    void tick();
    /// Diese Version führt s Sekunden Simulation durch anstatt Echtzeit (mal timelapse).
    void tick(float s);

    /**
     * Fügt Objekt der Welt hinzu und verwaltet es. Gibt die vergebene ID zurück.
     * @note Vergibt niemals die ID 0. Objekte mit ID 0 sind unfertig/fehlerhaft erzeugt worden
     *       und können so beim Debuggen aufgespürt werden.
     */
    oid_t add(Objekt* o);

    /// Erstellt ein neues Sub via `new` - manueller `delete` notwendig.
    const Sub* add_new_sub(uint8_t team, bool computer_controlled);

    /// Fügt einen Abschuss der Statistik hinzu.
    void add_abschuss(Abschuss&& abschuss);

    /**
     * Lässt `sub` mit `eingestelltes_torpedo` schießen, welches Zielkoordinaten usw. konfiguriert haben muss.
     * @note Hat `sub` keine Torpedos mehr des gewünschten Typs, passiert nichts.
     */
    bool add_torpedo(Sub* sub, const Torpedo* eingestelltes_torpedo);

    /**
     * Lässt `sub` mit `eingestellter_decoy` schießen, welches fertig konfiguriert sein sollte.
     * @note Hat `sub` keine Decoys mehr des gewünschten Typs, passiert nichts.
     */
    bool add_decoy(Sub* sub, const Decoy* eingestellter_decoy);

    /// Liefert den Zeitrafferfaktor. 1.0 bedeutet Echtzeit.
    float get_timelapse() const { return timelapse; }

    /// Liefert Team i. @note Nummerierung beginnt mit 1.
    const Team& get_team(uint8_t i) const { return teams.at(i); }

    /// Liefert die Anzahl Teams.
    size_t get_team_anzahl() const { return teams.size(); }

    /// Liefert alle  in der Welt simulierten Objekte.
    const std::unordered_map<oid_t, std::unique_ptr<Objekt>>& get_objekte() const { return objekte; }

    /// Liefert die Anzahl an Objekten von `typ`.
    unsigned get_objektanzahl(Objekt::Typ typ) const;

    /// Liefert Objekt mit `id` oder `nullptr`.
    const Objekt* get_objekt_or_null(oid_t id);

    /// Liefert alle eroberbaren Zonen.
    const std::vector<Zone>& get_zonen() const { return zonen; }

    /// Liefert das Terrain.
    const Karte& get_karte() const { return karte; }

private:

    /// Zeitraffer. TODO: Broadcast
    float timelapse = 1.0f; // TODO 1.0f wenn nicht test

    /// Tiefe, in der Subs gespawnt werden.
    float start_tiefe_sub = -50.f;

    /// Teams. TODO: Broadcast
    std::unordered_map<uint8_t, Team> teams;

    /// Alle simulierten Objekte, Subs, Spielersubs, KI-Subs, UUVs, Torpedos
    std::unordered_map<oid_t, std::unique_ptr<Objekt>> objekte;

    /// Punktezonen.
    std::vector<Zone> zonen;

    /// Statistik über alle Abschpsse.
    std::vector<Abschuss> abschuesse;

    /// Terrain der Welt.
    Karte karte;

};
