#include "game/team.hpp"
#include "game/objects/zone.hpp"

/// Simluierte Welt, die alle Objekte enthält.
class Welt final {

    /// Welt hat privaten Zugriff auf folgende Klassen:
    friend class Host;
    friend class Kommando;
    friend class Zone;
    friend class Sub_AI;
    friend class Sonar_Passiv;

public:

    /// Erstellt eine nutzbare Welt. Teams, Zonen und Computergegner werden auch generiert.
    explicit Welt(unsigned npcs_pro_team = 4);

    /// Dtor.
    ~Welt();

    /** 
     * Führt einen Simulationsschritt durch. Die Methode misst selbst die Zeit, wann sie davor zuletzt
     * aufgerufen wurde und skaliert entsprechend alle physikalischen Bewegungen etc.
     * */
    void tick();

    /// Fügt Objekt der Welt hinzu und verwaltet es.
    void add(Objekt* o);

    /// Erstellt ein neues Sub via `new` - manueller `delete` notwendig.
    const Sub* add_new_sub(uint8_t team, bool computer_controlled);

    /// Liefert den Zeitrafferfaktor. 1.0 bedeutet Echtzeit.
    float get_timelapse() const { return timelapse; }

    /// Liefert Team i. @note Nummerierung beginnt mit 1.
    const Team& get_team(uint8_t i) const { return teams.at(i); }

    /// Liefert die Anzahl Teams.
    size_t get_team_anzahl() const { return teams.size(); }

    /// Liefert alle  in der Welt simulierten Objekte.
    const std::unordered_map<uint32_t, Objekt*>& get_objekte() const { return objekte; }

    /// Liefert alle eroberbaren Zonen.
    const std::vector<Zone>& get_zonen() const { return zonen; }

private:

    /// Zeitraffer. TODO: Broadcast
    float timelapse = 1.0f; // TODO 1.0f wenn nicht test

    /// Teams. TODO: Broadcast
    std::unordered_map<uint8_t, Team> teams;

    /// Alle simulierten Objekte, Subs, Spielersubs, KI-Subs, UUVs, Torpedos
    std::unordered_map<uint32_t, Objekt*> objekte;

    /// Punktezonen.
    std::vector<Zone> zonen;

};
