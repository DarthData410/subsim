#include "game/team.hpp"

#include <OgreTimer.h>

class Welt final {

    friend class Server;

public:

    Welt();

    ~Welt();

    void tick();

    Sub get_new_player_sub(uint8_t team);

private:

    float timelapse = 1.0f;

    Ogre::Timer timer;

    std::unordered_map<uint8_t, Team> teams;

    std::unordered_map<uint32_t, Objekt*> objekte;

};
