#include "team.hpp"

#include <zufall.hpp>
#include <cassert>

Team::Team(uint8_t id) : id(id), punkte(1000) {
    const Motor m1_linear(5, 0.25);
    const Motor m1_rot(1, 0.1);
    const Motor m1_tauch(1, 0.1);
    Sub beginner_sub({0,0,0}, m1_linear, m1_rot, m1_tauch);
    beginner_sub.team = Team::id;
    beginner_sub.noise = 0.95f;
    const std::vector<std::tuple<float,float>> blindspots = {{135.f, -135.f}};
    beginner_sub.sonars.emplace_back(Sonar_Passiv(0.4f, 5.f, blindspots));
    const Torpedo torpedo1(Motor(10, 1),
                           Motor(5, 1),
                           Motor(5, 1),
                           "T1S", 10'000);
    const Torpedo torpedo2(Motor(20, 1),
                           Motor(5, 1),
                           Motor(5, 1),
                           "T1F", 5'000);
    beginner_sub.torpedos[torpedo1] = 4;
    beginner_sub.torpedos[torpedo2] = 4;
    sub_designs.push_back(beginner_sub);
}

const Sub& Team::get_new_sub() {
    assert(!sub_designs.empty());
    return Zufall::wahl(sub_designs);
}
