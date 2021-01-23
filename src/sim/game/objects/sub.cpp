#include "sub.hpp"

Sub::Sub(const Ogre::Vector3& pos,
         const Motor& motor_linear,const Motor& motor_rot, const Motor& motor_tauch)
         : Objekt_Steuerbar(pos, motor_linear, motor_rot, motor_tauch)
{
    //
}

void Sub::tick(Welt* welt, float s) {
    Objekt_Steuerbar::tick(welt, s);
}
