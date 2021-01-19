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

};
