#include <log.hpp>
#include "torpedo.hpp"
#include "sub.hpp"
#include "../../../sim/physik.hpp"
#include "../../welt.hpp"

Torpedo::Torpedo(const Motor& motor_linear, const Motor& motor_rot, const Motor& motor_tauch,
                 const std::string& name, float range, const Explosion& explosion,
                 const std::optional<Sonar_Aktiv>& sonar_aktiv,
                 const std::optional<Sonar_Passiv>& sonar_passiv)
        : Objekt_Steuerbar({0,0,0}, // unwichtig, wird überschrieben bei Kopie
                           motor_linear, motor_rot, motor_tauch,
                           1.0),
          name(name), range(range), sonar_aktiv(sonar_aktiv), sonar_passiv(sonar_passiv), explosion(explosion)
{
    //
}

Torpedo::Torpedo(const Torpedo& torpedo_typ, const Sub* sub,
                 float distance_to_activate, float target_bearing, float target_depth, float target_distance_to_explode)
    : Torpedo(torpedo_typ) // übernimmt alles aus torpedo_typ
{
    pos    = sub->get_pos(); // TODO etwas Abstand nach vorn
    kurs   = sub->get_bearing();
    quelle = sub->get_id();
    motor_linear.v = std::min(motor_linear.v_max, sub->get_speed() + 1.f);
    set_target_v(1.0f);
    set_target_bearing(target_bearing);
    set_target_depth(target_depth);
    Torpedo::distance_to_activate = distance_to_activate;
    Torpedo::distance_to_fuse = target_distance_to_explode;
}

bool Torpedo::tick(Welt* welt, float s) {
    const auto pos_alt = this->pos;
    if (!Objekt_Steuerbar::tick(welt, s)) return false;
    travelled += Physik::distanz(pos_alt.x(), pos_alt.y(), pos.x(), pos.y());
    if (travelled < distance_to_activate) return true; // noch nichts aktiv zu tun
    if (travelled > range) return false;
    // Nach gültigen Zielen suchen: Sonar-Passiv
    if (sonar_passiv) {
        sonar_passiv->tick(this, welt, s);
        if (const auto& ziel = get_beste_detektion(); ziel) {
            // Kurs auf Ziel stellen
            set_target_bearing(ziel->bearing);
        }
    }
    // Hit detection
    for (auto& o_paar : welt->objekte) {
        if (o_paar.second.get() == this) continue;
        if (!Physik::in_reichweite_xyz(this->pos, o_paar.second->get_pos(), distance_to_fuse)) continue; // zu weit

        // Noch näher möglich?
        const auto& o = o_paar.second;
        const auto distanz = Physik::distanz_xyz(this->pos, o->get_pos());
        if (distanz <= letzte_zielnaehe) { // noch näher
            letzte_zielnaehe = distanz;
            return true;
        } else { // entfernt sich -> BOOM!
            Log::debug() << "Torpedo " << this->name << " trifft Objekt " << o->get_id()
                         << " Typ=" << (int)o->get_typ() << ' ' << " Distanz: " << distanz << '\n';
            Explosion* e = new Explosion(this);
            welt->add(e);
            return false;
        }
    }
    return true;
}

const Detektion* Torpedo::get_beste_detektion() const {
    auto get_detektion = [this](const auto& sonar) {
        const auto& ziel = std::min_element(sonar.get_detektionen().begin(),
                                            sonar.get_detektionen().end(),
                            // Sortierung, die über die Zielpriorisierung entscheidet
                            [this](const Detektion& d1, const Detektion& d2) {
                                return std::abs(Physik::winkel_diff(this->kurs, d1.bearing)) <
                                       std::abs(Physik::winkel_diff(this->kurs, d2.bearing));
        });
        if (ziel != sonar.get_detektionen().end()) return &(*ziel);
        return (const Detektion*) nullptr;
    };
    // Priorität hat Detektion vom aktiven Sonar, dann vom passivem Sonar, wenn keine Detektionen: nullptr.
    const Detektion* beste_vom_aktiven  = nullptr;
    const Detektion* beste_vom_passiven = nullptr;
    if (sonar_aktiv)  beste_vom_aktiven  = get_detektion(sonar_aktiv.value());
    if (sonar_passiv) beste_vom_passiven = get_detektion(sonar_passiv.value());
    if (beste_vom_aktiven) return  beste_vom_aktiven;
    return beste_vom_passiven;
}

bool operator<(const Torpedo& lhs, const Torpedo& rhs) {
    return lhs.name < rhs.name;
}
