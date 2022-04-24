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
          name(name), range(range), explosion(explosion), sonar_aktiv(sonar_aktiv), sonar_passiv(sonar_passiv)
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
    if (sonar_aktiv) sonar_aktiv->set_mode(Sonar_Aktiv::Mode::ON);
}

bool operator<(const Torpedo& lhs, const Torpedo& rhs) {
    return lhs.name < rhs.name;
}

bool Torpedo::tick(Welt* welt, float s) {
    const auto pos_alt = this->pos;
    if (!Objekt_Steuerbar::tick(welt, s)) { // Wurde beschädigt?
        Explosion* e = new Explosion(this); // Dann explodiert es
        welt->add(e);
        return false;
    }
    travelled += Physik::distanz(pos_alt.x(), pos_alt.y(), pos.x(), pos.y());
    if (travelled < distance_to_activate) return true; // noch nichts aktiv zu tun
    if (travelled > range) {
        Log::debug() << "Torpedo " << id << " died out of range\n";
        return false; // Treibstoff leer -> deaktiviert
    }

    // Sonars arbeiten lassen
    if (sonar_aktiv)  sonar_aktiv->tick(this, welt, s);
    if (sonar_passiv) sonar_passiv->tick(this, welt, s);

    // Nach gültigen Zielen suchen:
    if (const auto& ziel = get_beste_detektion(); ziel) {
        // Kurs auf Ziel stellen
        set_target_bearing(ziel->bearing);
    }

    // Hit detection
    for (auto& o_paar : welt->objekte) {
        if (o_paar.second.get() == this) continue;

        // Ungültige Ziele überspringen
        if (o_paar.second.get()->get_typ() == Objekt::Typ::PING)      continue;
        if (o_paar.second.get()->get_typ() == Objekt::Typ::EXPLOSION) continue;

        // Noch näher möglich?
        if (!Physik::in_reichweite_xyz(this->pos, o_paar.second->get_pos(), distance_to_fuse)) continue; // zu weit
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
    std::vector<const Detektion*> detektionen; // alle Detektionen aller Sonars sammeln
    if (sonar_aktiv)  for (const auto& d :  sonar_aktiv->get_detektionen()) detektionen.push_back(&d);
    if (sonar_passiv) for (const auto& d : sonar_passiv->get_detektionen()) detektionen.push_back(&d);

    // Bestes Ziel Auswahl
    const auto& ziel = std::min_element(detektionen.begin(),
                                        detektionen.end(),
                        // Sortierung, die über die Zielpriorisierung entscheidet
                        [this](const Detektion* d1, const Detektion* d2) { // true = links / false = rechts
                            // 1) ACTIVE_SONAR_ECHO bevorzugen
                            if (d1->typ == Detektion::Typ::ACTIVE_SONAR_ECHO && d2->typ != Detektion::Typ::ACTIVE_SONAR_ECHO) return true;
                            if (d1->typ != Detektion::Typ::ACTIVE_SONAR_ECHO && d2->typ == Detektion::Typ::ACTIVE_SONAR_ECHO) return false;
                            // 2) MOVEMENT_SIGNATURE bevorzugen
                            if (d1->typ == Detektion::Typ::MOVEMENT_SIGNATURE && d2->typ != Detektion::Typ::MOVEMENT_SIGNATURE) return true;
                            if (d1->typ != Detektion::Typ::MOVEMENT_SIGNATURE && d2->typ == Detektion::Typ::MOVEMENT_SIGNATURE) return false;
                            // 3) Bei gleicher Richtung: näheres bevorzugen
                            if (d1->range.has_value() && d2->range.has_value() && d1->bearing == d2->bearing) return *d1->range < *d2->range;
                            // 4) Ähnlichere Richtung (div. Lautstärke) bevorzugen
                            //    Faktor 16 durch Test "Torpedo Zielauswahl via AS detektionen" in Test_Objekte ermittelt
                            return (std::abs(Physik::winkel_diff(this->kurs, d1->bearing)*16) / d1->gain) <
                                   (std::abs(Physik::winkel_diff(this->kurs, d2->bearing)*16) / d2->gain);
    });
    if (ziel != detektionen.end()) return *ziel;
    return (const Detektion*) nullptr;
}

bool Torpedo::apply_damage(const Explosion* explosion, float damage) {
    (void) explosion;
    if (damage <= 0) return false;
    // Totalschaden
    if (schaeden.count(Schaden::ZERSTOERT)) return false; // war bereits zerstört
    Log::debug() << "Torpedo " << this->get_id() << " durch Explosion zerstoert.\n";
    schaeden.insert(Schaden::ZERSTOERT);
    return true;
}
