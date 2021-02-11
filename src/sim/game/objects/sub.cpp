#include "sub.hpp"

Sub::Sub(const Ogre::Vector3& pos,
         const Motor& motor_linear,const Motor& motor_rot, const Motor& motor_tauch)
         : Objekt_Steuerbar(pos, motor_linear, motor_rot, motor_tauch), tarnung(1.0f)
{
    //
}

bool Sub::tick(Welt* welt, float s) {
    Objekt_Steuerbar::tick(welt, s);
    for (Sonar_Passiv& sonar_passiv : sonars) sonar_passiv.tick(this, welt, s);
    return true;
}

float Sub::get_noise() const {
    return Objekt_Steuerbar::get_noise() * tarnung;
}
