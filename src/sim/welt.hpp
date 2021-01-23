#include "game/team.hpp"

#include <OgreTimer.h>

class Welt final {

    friend class Spielszene;

public:

    Welt();

    ~Welt();

    void tick();

private:

    float timelapse = 1.0f;

    Ogre::Timer timer;

    std::vector<Team> teams;

    std::unordered_map<decltype(std::declval<Objekt>().get_id()), std::unique_ptr<Objekt>> objekte;

};
