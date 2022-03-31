#include "welt.hpp"
#include "game/objekte/sub_ai.hpp"

#include <zufall.hpp>

Welt::Welt(unsigned npcs_pro_team) {
    // Teams hinzufügen
    for (uint8_t i = 1; i <= 2; ++i) {
        teams[i] = Team(i);
        teams[i].basis = {i % 2 == 0 ? 1'500.f : -1'500.f, 0.f};
        for (unsigned k = 0; k < npcs_pro_team; ++k) add_new_sub(i, true);
    }
    // Punktezonen hinzufügen
    zonen.emplace_back(std::tuple(0.f,0.f), 1000.f);
}

Welt::~Welt() {
    for (auto& paar : objekte) delete paar.second;
}

void Welt::tick() {
    static sf::Clock timer;
    const float s = timelapse * timer.getElapsedTime().asSeconds(); // Sekunden vergangen
    timer.restart();
    tick(s);
}

void Welt::tick(float s) {
    std::unordered_set<decltype(objekte)::key_type> tote_objekte;
    for (auto& objekt : objekte) if (objekt.second->tick(this, s) == false) tote_objekte.insert(objekt.first);
    for (const auto key : tote_objekte) {
        delete objekte.at(key);
        objekte.erase(key);
    }
    // Ticks - Zonen
    for (auto& zone : zonen) zone.tick(this, s);
}

const Sub* Welt::add_new_sub(uint8_t team, bool computer_controlled) {
    Log::debug() << "Welt::" << __func__ << " for team " << (unsigned)team << Log::endl;
    Sub* sub_ptr = nullptr;
    if (computer_controlled) sub_ptr = new Sub_AI(teams.at(team).get_new_sub());
    else sub_ptr = new Sub(teams.at(team).get_new_sub());
    add(sub_ptr);
    sub_ptr->pos = { // Startposition beim Team, leicht versetzt
            std::get<0>(teams[team].get_pos()) + Zufall::f(-100.f, 100.f),
            start_tiefe_sub, // Tiefe
            std::get<1>(teams[team].get_pos()) + Zufall::f(-100.f, 100.f)
    };
    return sub_ptr;
}

uint32_t Welt::add(Objekt* o) {
    do o->regenerate_id(); // TODO Limit
    while (objekte.count(o->get_id()) || o->get_id() == 0);
    objekte[o->get_id()] = o; // Objekt in die Welt gesetzt
    return o->get_id();
}

bool Welt::shoot_torpedo(Sub* sub, const Torpedo& eingestelltes_torpedo) {
    if (sub->shoot(eingestelltes_torpedo.get_name())) {
        this->add(new Torpedo(eingestelltes_torpedo, sub,
                              eingestelltes_torpedo.get_distance_to_activate(),
                              eingestelltes_torpedo.get_target_bearing(),
                              eingestelltes_torpedo.get_target_depth()));
        Log::debug() << "Torpedo name=" << eingestelltes_torpedo.get_name() << " launched by Sub ID=" << sub->get_id() << '\n';
        return true;
    }
    Log::debug() << "Sub ID=" << sub->get_id() << " out of Ammo name=" << eingestelltes_torpedo.get_name() << '\n';
    return false;
}
