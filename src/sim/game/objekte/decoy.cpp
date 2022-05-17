#include <nada/log.hpp>
#include "decoy.hpp"
#include "sub.hpp"

Decoy::Decoy(const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch,
             const std::string& name, float noise, float lebenszeit_in_s) :
        Objekt_Steuerbar({0,0,0}, motor_linear, motor_rot, motor_tauch, noise),
        name(name), restzeit(lebenszeit_in_s)
{

}

Decoy::Decoy(const Decoy& decoy_typ, const Sub* sub) : Decoy(decoy_typ) {
    this->pos = sub->get_pos();
}

bool operator<(const Decoy& lhs, const Decoy& rhs) {
    return lhs.name < rhs.name;
}

bool Decoy::tick(Welt* welt, float s) {
    if (restzeit -= s; restzeit <= 0) { nada::Log::debug() << "Decoy " << id << " ran out and died\n"; return false; }
    Objekt_Steuerbar::tick(welt, s);
    return true;
}
