#include "team.hpp"
#include <zufall.hpp>

Team::Team(uint8_t id) : id(id), punkte(1000) {
    Sub beginner_sub({0,0,0},
            Motor(4, 0.5),
            Motor(1, 0.5),
            Motor(1, 0.5)
    );
    beginner_sub.team = Team::id;
    beginner_sub.tarnung = 1.0f;
    beginner_sub.sonars.emplace_back(0.25f, 270, 180);
    sub_designs.push_back(beginner_sub);
}

const Sub& Team::get_new_sub() {
    assert(!sub_designs.empty());
    return Zufall::wahl(sub_designs);
}
