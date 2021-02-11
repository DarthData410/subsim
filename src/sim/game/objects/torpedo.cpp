#include "torpedo.hpp"
#include "../../../sim/physik.hpp"

Torpedo::Torpedo(const Torpedo& torpedo_typ, float distance_to_activate, float target_bearing, float target_depth)
    : name(torpedo_typ.name), range(torpedo_typ.range),
    distance_to_activate(distance_to_activate), target_bearing(target_bearing), target_depth(target_depth)
{
    //
}

bool Torpedo::tick(Welt* welt, float s) {
    const auto pos_alt = this->pos;
    Objekt_Steuerbar::tick(welt, s);
    travelled += Physik::distanz(pos_alt.x, pos_alt.z, pos.x, pos.z);
    if (travelled > range) return false; // Keine Reichweite mehr

    //if (travelled > distance_to_activate)
    return true;
}

bool operator<(const Torpedo& lhs, const Torpedo& rhs) {
    return lhs.name < rhs.name;
}
