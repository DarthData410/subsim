#include <OgreQuaternion.h>

class Physik final {

public:

    static void set_pitch(Ogre::Quaternion& q, float target_pitch);

    static void rotate(Ogre::Quaternion& q, float degree);

    static void move_ahead(Ogre::Vector3& pos, const Ogre::Quaternion& q, float amount);

protected:



};
