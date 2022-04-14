#include <log.hpp>
#include "sub.hpp"

Sub::Sub(const std::string& name, const Vektor& pos,
         const Motor& motor_linear,const Motor& motor_rot, const Motor& motor_tauch)
         : Objekt_Steuerbar(pos, motor_linear, motor_rot, motor_tauch), name(name)
{
    //
}

bool Sub::tick(Welt* welt, float s) {
    if (!Objekt_Steuerbar::tick(welt, s)) return false;
    if (!welt) return true; // alles weitere nur Host
    for (Sonar_Passiv& sonar_passiv : sonars_passive) sonar_passiv.tick(this, welt, s);
    return true;
}

bool Sub::shoot(const std::string& torpedo_name) {
    for (auto& paar : torpedos) if (paar.first.get_name() == torpedo_name) {
        if (paar.second > 0) { paar.second--; return true; }
        else return false;
    }
    Log::err() << "Sub::shoot(): no Torpedo with name=" << torpedo_name << '\n';
    return false;
}

dist_t Sub::get_max_reichweite_as() const {
    dist_t max_range = 0;
    for (const auto& as : sonars_active) if (as.get_max_range() > max_range) max_range = as.get_max_range();
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
