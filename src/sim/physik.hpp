#include <OgreQuaternion.h>

class Physik final {

public:

    static void set_pitch(Ogre::Quaternion& q, float target_pitch);

    static void rotate(Ogre::Quaternion& q, float degree);

    static void move_ahead(Ogre::Vector3& pos, const Ogre::Quaternion& q, float amount);

    /// Liefert die kürzeste Rotation (in Grad) von Winkel 1 zu Winkel 2.
    static float rotation(float winkel1, float winkel2);

    /// Liefert den Winkel von Punkt (x,y) zu Punkt (target_x,target_y).
    static float bearing(float x, float y, float target_x, float target_y);

    /// Liefert den Winkel in der y-Ebene (d.h. Höhe/Tiefe) zwischen zwei 3D-Punkten.
    static float winkel_tiefe(const Ogre::Vector3& pos, const Ogre::Vector3& target_pos);

    /// Liefert die Distanz zwischen zwei 2D-Koordinaten.
    static float distanz(float x1, float y1, float x2, float y2);

    /// Liefert die Distanz zwischen zwei 3D-Koordinaten.
    static float distanz(const Ogre::Vector3& pos, const Ogre::Vector3& target_pos);

    static float bremsweg(float v, float a);

};
