#include "game/team.hpp"
#include "game/objects/zone.hpp"

#include <OgreTimer.h>

class Welt final {

    friend class Host;
    friend class Kommando;
    friend class Zone;
    friend class Sub_AI;

public:

    Welt();

    ~Welt();

    void tick();

    Sub get_new_player_sub(uint8_t team);

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
