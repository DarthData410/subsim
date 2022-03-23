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
    Welt();

    /// Dtor.
    ~Welt();

    /** 
     * Führt einen Simulationsschritt durch. Die Methode misst selbst die Zeit, wann sie davor zuletzt
     * aufgerufen wurde und skaliert entsprechend alle physikalischen Bewegungen etc.
     * */
    void tick();

    /// Erstellt ein neues Sub via `new` - manueller `delete` notwendig.
    const Sub* get_new_sub(uint8_t team, bool computer_controlled);

private:

    /// Zeitraffer. TODO: Broadcast
    float timelapse = 1.0f;

    /// Teams. TODO: Broadcast
    std::unordered_map<uint8_t, Team> teams;

    /// Alle simulierten Objekte, Subs, Spielersubs, KI-Subs, UUVs, Torpedos
    std::unordered_map<uint32_t, Objekt*> objekte;

    /// Punktezonen.
    std::vector<Zone> zonen;

};
