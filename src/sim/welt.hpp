#include <OgreTimer.h>

class Welt final  {

public:

    Welt();

    virtual ~Welt();

    void tick();

private:

    float timelapse = 1.0f;

    Ogre::Timer timer;

};
