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
    const std::vector<std::tuple<float,float>> blindspots = {{135.f, -135.f}};
    const Sonar_Passiv sonar_passiv1(0.4f, 5.f, blindspots);
    beginner_sub.sonars.emplace_back(sonar_passiv1);

    // Stock Torpedo
    const Torpedo torpedo1(Motor(10, 1),
                           Motor(20, 1),
                           Motor(10, 1),
                           "T1A",
                           10'000,
                           Explosion(30.f, 50, 10),
                           sonar_passiv1);
    beginner_sub.torpedos[torpedo1] = 6;
    sub_designs.push_back(beginner_sub);
}

const Sub& Team::get_new_sub() {
    assert(!sub_designs.empty());
    return Zufall::wahl(sub_designs);
}
