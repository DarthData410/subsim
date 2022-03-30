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
    friend class Test_Welt;

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
    /// Diese Version führt s Sekunden Simulation durch anstatt Echtzeit (mal timelapse).
    void tick(float s);

    /**
     * Fügt Objekt der Welt hinzu und verwaltet es. Gibt die vergebene ID zurück.
     * @note Vergibt niemals die ID 0. Objekte mit ID 0 sind unfertig/fehlerhaft erzeugt worden
     *       und können so beim Debuggen aufgespürt werden.
     */
    uint32_t add(Objekt* o);

    /// Erstellt ein neues Sub via `new` - manueller `delete` notwendig.
    const Sub* add_new_sub(uint8_t team, bool computer_controlled);

    /**
     * Lässt `sub` mit `eingestelltes_torpedo` schießen, welches Zielkoordinaten usw. konfiguriert haben muss.
     * @note Hat `sub` keine Torpedos mehr des gewünschten Typs, passiert nichts.
     */
    bool shoot_torpedo(Sub* sub, const Torpedo& eingestelltes_torpedo);

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

    /// Tiefe, in der Subs gespawnt werden.
    float start_tiefe_sub = -50.f;

    /// Teams. TODO: Broadcast
    std::unordered_map<uint8_t, Team> teams;

    /// Alle simulierten Objekte, Subs, Spielersubs, KI-Subs, UUVs, Torpedos
    std::unordered_map<uint32_t, Objekt*> objekte;

    /// Punktezonen.
    std::vector<Zone> zonen;

};
