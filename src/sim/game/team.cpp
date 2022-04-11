#include "team.hpp"

#include <zufall.hpp>
#include <cassert>

Team::Team(uint8_t id) : id(id), punkte(1000) {
    // Stock Sub
    const Motor m1_linear(4, 0.25);
    const Motor m1_rot(1, 0.1);
    const Motor m1_tauch(1, 0.1);
    Sub beginner_sub({0,0,0}, m1_linear, m1_rot, m1_tauch);
    beginner_sub.team = Team::id;
    beginner_sub.noise = 0.95f;

    // Stock Passiver Sonar
    const std::vector<std::tuple<float,float>> blindspots1 = {{180, 170}}; // 10° links/rechts vorn sichtbar
    const std::vector<std::tuple<float,float>> blindspots2 = {{180,  90}}; // 90° links/rechts vorn sichtbar (=vordere Hälfte)
    const Sonar_Aktiv  sonar_aktiv_t(15.f, 10'000, 10.f, blindspots1); // für Torpedos
    const Sonar_Aktiv  sonar_aktiv_s(15.f, 10'000, 10.f, blindspots2); // für Subs
    const Sonar_Passiv sonar_passiv_t(0.4f, 15.f, blindspots1); // für Torpedos
    const Sonar_Passiv sonar_passiv_s(0.4f, 15.f, blindspots2); // für Subs
    //const Sonar_Passiv sonar_passiv_s(0.4f, 20.f, blindspots3);
    beginner_sub.sonars_active.emplace_back(sonar_aktiv_s);
    beginner_sub.sonars_passive.emplace_back(sonar_passiv_s);

    // Stock Torpedo
    const Torpedo torpedo1(Motor(10, 1),
                           Motor(20, 1),
                           Motor(10, 1),
                           "T1A",
                           10'000,
                           Explosion(30.f, 50, 10),
                           sonar_aktiv_t,
                           sonar_passiv_t);
    beginner_sub.torpedos[torpedo1] = 6;
    sub_designs.push_back(beginner_sub);
}

const Sub& Team::get_new_sub() {
    return Zufall::wahl(sub_designs);
}
