#include <log.hpp>
#include "sub.hpp"

Sub::Sub(const Vektor& pos,
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

bool Sub::shoot(const std::string& torpedo_name) {
    for (auto& paar : torpedos) if (paar.first.get_name() == torpedo_name) {
        if (paar.second > 0) { paar.second--; return true; }
        else return false;
    }
    Log::err() << "Sub::shoot(): no Torpedo with name=" << torpedo_name << '\n';
    return false;
}
