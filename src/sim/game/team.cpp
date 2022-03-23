#include "team.hpp"

#include <zufall.hpp>
#include <cassert>

Team::Team(uint8_t id) : id(id), punkte(1000) {
    const Motor m1_linear(4, 0.5);
    const Motor m1_rot(2, 0.5);
    const Motor m1_tauch(1, 0.25);
    Sub beginner_sub({0,0,0}, m1_linear, m1_rot, m1_tauch);
    beginner_sub.team = Team::id;
    beginner_sub.tarnung = 1.0f;
    beginner_sub.sonars.emplace_back(0.25f, 270, 180);
    const Torpedo torpedo1(Motor(10, 1),
                           Motor(5, 1),
                           Motor(5, 1),
                           "T1S", 1000);
    const Torpedo torpedo2(Motor(20, 1),
                           Motor(5, 1),
                           Motor(5, 1),
                           "T1F", 500);
    beginner_sub.torpedos[torpedo1] = 4;
    beginner_sub.torpedos[torpedo2] = 4;
    sub_designs.push_back(beginner_sub);
}

const Sub& Team::get_new_sub() {
    assert(!sub_designs.empty());
    return Zufall::wahl(sub_designs);
}
