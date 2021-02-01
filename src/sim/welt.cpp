#include "welt.hpp"

Welt::Welt() {
    // Teams hinzufügen
    for (uint8_t i = 1; i <= 2; ++i) {
        teams[i] = Team(i);
        teams[i].basis = {i % 2 == 0 ? 10'000.f : -10'000.f, 0.f};
    }
    // Punktezonen hinzufügen
    zonen.emplace_back(std::tuple(0.f,0.f), 1000.f);
}

Welt::~Welt() {
    for (auto& paar : objekte) delete paar.second;
}

void Welt::tick() {
    // Timing
    static Ogre::Timer timer;
    const float s = timelapse * timer.getMilliseconds() / 1000.f; // Sekunden vergangen
    timer.reset();

    // Ticks
    for (auto& objekt : objekte) objekt.second->tick(this, s);
    for (auto& zone : zonen) zone.tick(this, s);
}

Sub Welt::get_new_player_sub(uint8_t team) {
    Sub* sub_ptr = new Sub(teams.at(team).get_new_sub());
    objekte[sub_ptr->get_id()] = sub_ptr;
    sub_ptr->pos = { // Startposition beim Team
            std::get<0>(teams[team].get_pos()),
            sub_ptr->pos.y,
            std::get<1>(teams[team].get_pos())
    };
    return *sub_ptr;
}
