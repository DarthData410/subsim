#include "explosion.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

Explosion::Explosion(const Vektor& pos, const float& bearing, Objekt::oid_t source, dist_t radius, float power,
                     float remaining_time)
        : Objekt(pos, bearing), source(source), radius(radius), power(power), remaining_time(remaining_time)
{

}

bool Explosion::tick(Welt* welt, float s) {
    /// Objekten Schaden zufügen
    if (!damage_done) {
        for (auto& paar : welt->get_objekte()) {
            Objekt* o = paar.second;
            if (this == o) continue;
            if (!Physik::in_reichweite_xyz(this->get_pos(), o->get_pos(), radius)) continue;
            const double d = Physik::distanz_xyz(this->get_pos(), o->get_pos());
            const double range_faktor = 1.0 - (d / radius);
            const float damage = range_faktor * power;
            o->apply_damage(this, damage);
        }
        damage_done = true;
    }

    /// Explosion noch Sichtbar?
    remaining_time -= s;
    return remaining_time > 0;
}
