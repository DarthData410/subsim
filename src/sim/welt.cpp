#include "welt.hpp"
#include "game/objekte/sub_ai.hpp"

#include <zufall.hpp>

Welt::Welt(unsigned npcs_pro_team) {
    // Teams hinzufügen
    for (uint8_t i = 1; i <= 2; ++i) {
        teams[i] = Team(i);
        teams[i].basis = {i % 2 == 0 ? 15000.f : -15000.f, 0.f, 0};
        for (unsigned k = 0; k < npcs_pro_team; ++k) add_new_sub(i, true);
    }
    // Punktezonen hinzufügen
    zonen.emplace_back(std::tuple(0.f,0.f), 2500.f);
}
void Welt::tick() {
    static sf::Clock timer;
    const float s = timelapse * timer.getElapsedTime().asSeconds(); // Sekunden vergangen
    timer.restart();
    tick(s);
    // Benchmark
    static unsigned counter = 0;
    static float counter_seconds = 0.f;
    counter++;
    counter_seconds += s;
    if (counter_seconds > 10) {
        Log::out() << "Welt::tick @ " << std::round(static_cast<float>(counter) / counter_seconds) << " ticks/s\n";
        counter = 0;
        counter_seconds = 0;
    }
}

void Welt::tick(float s) {
    std::unordered_set<decltype(objekte)::key_type> tote_objekte;
    for (auto& objekt : objekte) if (objekt.second->tick(this, s) == false) tote_objekte.insert(objekt.first);
    for (const auto key : tote_objekte) objekte.erase(key);
    for (auto& zone : zonen) zone.tick(this, s);
}

const Sub* Welt::add_new_sub(uint8_t team, bool computer_controlled) {
    Log::debug() << "Welt::" << __func__ << " for team " << (unsigned)team << Log::endl;
    Sub* sub_ptr = nullptr;
    if (computer_controlled) sub_ptr = new Sub_AI(teams.at(team).get_new_sub());
    else sub_ptr = new Sub(teams.at(team).get_new_sub());
    add(sub_ptr);
    sub_ptr->pos = { // Startposition beim Team, leicht versetzt
            teams[team].get_basis().x() + Zufall::f(-500.f, 500.f),
            teams[team].get_basis().y() + Zufall::f(-500.f, 500.f),
            start_tiefe_sub // Tiefe
    };
    return sub_ptr;
}

oid_t Welt::add(Objekt* o) {
    do o->regenerate_id(); // TODO Limit
    while (objekte.count(o->get_id()) || o->get_id() == 0);
    objekte[o->get_id()] = std::unique_ptr<Objekt>(o); // Objekt in die Welt gesetzt
    return o->get_id();
}

bool Welt::add_torpedo(Sub* sub, const Torpedo& eingestelltes_torpedo) {
    if (sub->shoot(eingestelltes_torpedo.get_name())) {
        this->add(new Torpedo(eingestelltes_torpedo, sub,
                              eingestelltes_torpedo.get_distance_to_activate(),
                              eingestelltes_torpedo.get_target_bearing(),
                              eingestelltes_torpedo.get_target_depth(),
                              eingestelltes_torpedo.get_distance_to_explode()));
        Log::debug() << "Torpedo name=" << eingestelltes_torpedo.get_name() << " launched by Sub ID=" << sub->get_id() << '\n';
        return true;
    }
    Log::debug() << "Sub ID=" << sub->get_id() << " out of Ammo name=" << eingestelltes_torpedo.get_name() << '\n';
    return false;
}

void Welt::add_abschuss(Abschuss&& abschuss) {
    abschuesse.push_back(std::move(abschuss));
}

const Objekt* Welt::get_objekt_or_null(oid_t id) {
    if (objekte.count(id) && objekte[id]) return objekte[id].get();
    return nullptr;
}
