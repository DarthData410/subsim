#include <log.hpp>
#include "explosion.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"
#include "../abschuss.hpp"

Explosion::Explosion(dist_t radius, float power, float remaining_time,
                     const Vektor& pos, float bearing, oid_t source)
        : Objekt(pos, bearing), quelle_sub(source), radius(radius), power(power), remaining_time(remaining_time)
{

}

Explosion::Explosion(const Torpedo* torpedo) : Explosion(torpedo->explosion) {
    pos            = torpedo->pos;
    quelle_sub     = torpedo->quelle;
    quelle_torpedo = torpedo->get_id();
    kurs   = 0;
    pitch  = 0;
}

bool Explosion::tick(Welt* welt, float s) {
    /// Objekten Schaden zufügen
    if (!damage_done) {
        for (auto& paar : welt->get_objekte()) {
            // Gültiges Zielobjekt?
            Objekt* o = paar.second.get();
            if (this == o) continue;
            if (o->get_typ() == Typ::EXPLOSION) continue;
            if (o->get_typ() == Typ::PING)      continue;
            if (!Physik::in_reichweite_xyz(this->get_pos(), o->get_pos(), radius)) continue;

            // Schaden anwenden
            const double d = Physik::distanz_xyz(this->get_pos(), o->get_pos());
            const double range_faktor = std::max(1.0 - (d / radius), 0.01);
            const float damage = range_faktor * power;
            Log::debug() << "Explosion beschaedigt Objekt " << o->get_id() << " Typ=" << (int)o->get_typ()
                         << " Schaden=" << damage << " (Max=" << power << ")\n";

            // Schaden zufügen
            if (o->apply_damage(this, damage) && o->get_id() != quelle_torpedo) {
                // Abschuss? Dann in die Statistik (außer wenn "Quell"-torpedo)
                welt->add_abschuss(std::move(Abschuss(
                        welt->get_objekt_or_null(quelle_sub),
                        welt->get_objekt_or_null(o->get_id()),
                        nullptr))
                );
            }
        }
        damage_done = true;
    }

    /// Explosion noch Sichtbar?
    remaining_time -= s;
    return remaining_time > 0;
}

const std::string& Explosion::get_name() const {
    static const std::string s("Explosion");
    return s;
}
