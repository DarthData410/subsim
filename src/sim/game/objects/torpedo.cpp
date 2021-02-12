#include "torpedo.hpp"
#include "../../../sim/physik.hpp"
#include "sub.hpp"

Torpedo::Torpedo(const Torpedo& torpedo_typ, const Sub* sub, float distance_to_activate, float target_bearing, float target_depth)
    : name(torpedo_typ.name), range(torpedo_typ.range),
    distance_to_activate(distance_to_activate), target_bearing(target_bearing), target_depth(target_depth)
{
    pos = sub->get_pos();
    motor_linear.v = std::min(motor_linear.v_max, sub->get_speed() + 1.f);
    set_target_v(1.0f);
    set_target_bearing(target_bearing);
    // TODO target_depth
}

bool Torpedo::tick(Welt* welt, float s) {
    const auto pos_alt = this->pos;
    Objekt_Steuerbar::tick(welt, s);
    travelled += Physik::distanz(pos_alt.x, pos_alt.z, pos.x, pos.z);
    if (travelled > range) return false; // Keine Reichweite mehr
    if (travelled < distance_to_activate) set_target_bearing(target_bearing);
    else { // Aktiv
        // TODO seeker
    }
    // TODO seeker
    // TODO hit detect
    return true;
}

bool operator<(const Torpedo& lhs, const Torpedo& rhs) {
    return lhs.name < rhs.name;
}
