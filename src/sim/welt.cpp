#include "welt.hpp"
#include "game/objekte/sub_ai.hpp"

#include <nada/random.hpp>

using nada::Log;

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
    const float s = timer.getElapsedTime().asSeconds();
    timer.restart();
    tick(s * timelapse);
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
    Sub* sub_ptr;
    if (computer_controlled) sub_ptr = new Sub_AI(teams.at(team).get_new_sub());
    else sub_ptr = new Sub(teams.at(team).get_new_sub());
    add(sub_ptr);
    const auto& [start_x, start_y] = karte.get_nearest_passable(
            teams[team].get_basis().x() + nada::random::f(-500.f, 500.f),
            teams[team].get_basis().y() + nada::random::f(-500.f, 500.f),
            START_DEPTH_SUB, std::less<>());
    sub_ptr->pos = {start_x, start_y, START_DEPTH_SUB};
    const auto depth_at_start = karte.get_height_at(start_x, start_y);
    if (depth_at_start > START_DEPTH_SUB) throw std::runtime_error("Error: Depth for new sub = " + std::to_string(depth_at_start));
    else Log::debug() << "Welt::add_new_sub depth=" << depth_at_start << '\n';
    return sub_ptr;
}

oid_t Welt::add(Objekt* o) {
    do o->regenerate_id(); // TODO Limit
    while (objekte.count(o->get_id()) || o->get_id() == 0);
    objekte[o->get_id()] = std::unique_ptr<Objekt>(o); // Objekt in die Welt gesetzt
    return o->get_id();
}

std::optional<oid_t> Welt::add_torpedo(Sub* sub, const Torpedo* eingestelltes_torpedo) {
    if (sub->shoot(eingestelltes_torpedo->get_name(), Objekt::Typ::TORPEDO)) {
        Torpedo* torpedo = new Torpedo(*eingestelltes_torpedo, sub,
                                       eingestelltes_torpedo->get_distance_to_activate(),
                                       eingestelltes_torpedo->get_target_bearing(),
                                       eingestelltes_torpedo->get_target_depth(),
                                       eingestelltes_torpedo->get_distance_to_explode());
        const auto torpedo_id = this->add(torpedo);
        Log::debug() << "Welt::add_torpedo() Torpedo name=" << eingestelltes_torpedo->get_name() << " launched by Sub ID=" << sub->get_id() << '\n';
        return torpedo_id;
    }
    return std::nullopt;
}

std::optional<oid_t> Welt::add_decoy(Sub* sub, const Decoy* eingestellter_decoy) {
    if (sub->shoot(eingestellter_decoy->get_name(), Objekt::Typ::DECOY)) {
        Decoy* decoy = new Decoy(*eingestellter_decoy, sub);
        const auto oid = this->add(decoy);
        Log::debug() << "Welt::add_decoy() Decoy name=" << eingestellter_decoy->get_name() << " launched by Sub ID=" << sub->get_id() << '\n';
        return oid;
    }
    return std::nullopt;
}

void Welt::add_abschuss(Abschuss&& abschuss) {
    abschuesse.push_back(std::move(abschuss));
}

const Objekt* Welt::get_objekt_or_null(oid_t id) const {
    if (objekte.count(id) && objekte.at(id)) return objekte.at(id).get();
    return nullptr;
}

unsigned Welt::get_objektanzahl(Objekt::Typ typ) const {
    return std::count_if(objekte.begin(), objekte.end(), [&](const auto& paar) {
        return paar.second->get_typ() == typ;
    });
}
