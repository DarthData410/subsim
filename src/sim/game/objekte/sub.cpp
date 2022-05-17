#include "sub.hpp"
#include "../../welt.hpp"

#include <nada/log.hpp>

using nada::Log;

Sub::Sub(const std::string& name, const Vektor& pos,
         const Motor& motor_linear,const Motor& motor_rot, const Motor& motor_tauch)
         : Objekt_Steuerbar(pos, motor_linear, motor_rot, motor_tauch), name(name)
{
    //
}

bool Sub::tick(Welt* welt, float s) {
    if (!Objekt_Steuerbar::tick(welt, s)) return false;
    if (this->pos.z() > Sub::SURFACE_DEPTH) this->pos.z(Sub::SURFACE_DEPTH); // Praktisch aufgetaucht
    if (!welt) return true; // alles weitere kann nur Host mit `welt`
    for (Sonar_Aktiv&  as : sonars_active)  as.tick(this, welt, s);
    for (Sonar_Passiv& ps : sonars_passive) ps.tick(this, welt, s);
    return true;
}

bool Sub::shoot(const std::string& weapon_name, Objekt::Typ torpedo_oder_gegenmassnahme) {
    auto get_weapon = [&](auto& container) {
        for (auto& paar : container) if (paar.first.get_name() == weapon_name) {
            if (paar.second > 0) {
                paar.second--;
                return true;
            }
            else {
                Log::err() << "Sub::shoot(): " << name << " is out of ammo\n";
                return false;
            }
        }
        Log::err() << "Sub::shoot(): Unknown Object " << (int)torpedo_oder_gegenmassnahme << " with name=" << name << '\n';
        return false;
    };
    if (torpedo_oder_gegenmassnahme != Objekt::Typ::TORPEDO && torpedo_oder_gegenmassnahme != Objekt::Typ::DECOY) {
        Log::err() << "Sub::shoot(): Invalid Weapontype: " << (int)torpedo_oder_gegenmassnahme << " with name=" << name << '\n';
        return false;
    }
    const bool abschuss_erfolgt = torpedo_oder_gegenmassnahme == Objekt::Typ::TORPEDO ?
            get_weapon(torpedos) : get_weapon(decoys);
    return abschuss_erfolgt;
}

dist_t Sub::get_max_reichweite_as() const {
    dist_t max_range = 0;
    for (const auto& as : sonars_active) max_range = std::max(max_range, as.get_max_range());
    return max_range;
}

float Sub::get_max_reichweite_torpedo(bool nur_mit_muni) const {
    float max_range = 0;
    for (const auto& t : torpedos) if (t.second > 0 || nur_mit_muni == false) max_range = std::max(max_range, t.first.get_range());
    return max_range;
}

float Sub::get_beste_empfindlichkeit_ps() const {
    float best = 1.0;
    for (const auto& ps : sonars_passive) if (ps.get_empfindlichkeit() < best) best = ps.get_empfindlichkeit();
    return best;
}

float Sub::get_best_aufloesung_sonar() const {
    float best = 999;
    for (const auto& as  : sonars_active)  if (as.get_aufloesung() < best) best = as.get_aufloesung();
    for (const auto& ps : sonars_passive) if (ps.get_aufloesung() < best) best = ps.get_aufloesung();
    return best;
}

std::optional<oid_t> Sub::deploy_decoy(Welt* welt, const std::string& decoy_key) {
    for (auto& [decoy, anzahl] : decoys) {
        if (decoy.get_name() == decoy_key) {
            anzahl -= 1;
            const auto oid = welt->add_decoy(this, &decoy);
            return oid;
        }
    }
    return std::nullopt;
}
