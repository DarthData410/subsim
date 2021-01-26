#include "welt.hpp"

Welt::Welt() {
    // Teams hinzufügen
    for (uint8_t i = 1; i <= 2; ++i) teams[i] = {i};
}

Welt::~Welt() {
    for (auto& paar : objekte) delete paar.second;
}

void Welt::tick() {
    // Timing
    const float s = timer.getMilliseconds() / 1000.f; // Sekunden vergangen
    timer.reset();

    for (const auto& objekt : objekte) objekt.second->tick(this, s);
}

Sub Welt::get_new_player_sub(uint8_t team) {
    Sub* sub_ptr = new Sub(teams.at(team).get_new_sub());
    objekte[sub_ptr->get_id()] = sub_ptr;
    return *sub_ptr;
}
