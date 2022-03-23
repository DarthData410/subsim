#include "zone.hpp"
#include "../../welt.hpp"

#include <tuple>
#include <SFML/System/Clock.hpp>

Zone::Zone(const std::tuple<double, double>& pos, float groesse) : team(0), pos(pos), groesse(groesse)
{
    //
}

void Zone::tick(Welt* welt, float s) {
    // Punkte an Team geben
    static float s_counter = 0.f;
    if (s_counter += s; s_counter >= 1.0f) {
        s_counter = 0.f;
        if (team > 0) welt->teams[team].punkte += 1;
    }
    // Besitzer neu bestimmen
    if (static sf::Clock timer; timer.getElapsedTime().asMilliseconds() > 10'000) {
        timer.restart();
        std::unordered_map<uint8_t, unsigned> team_subs; // Team / Anzahl Subs in der Zone
        for (const auto& paar : welt->objekte) {
            const Objekt* o = paar.second;
            if (o->get_typ() == Objekt::Typ::SUB) {
                const Sub* sub = (Sub*) o;
                const auto diff_x = std::abs(sub->get_pos().x() - std::get<0>(pos));
                const auto diff_y = std::abs(sub->get_pos().y() - std::get<1>(pos));
                if (diff_x <= groesse && diff_y <= groesse) {
                    team_subs[sub->get_team()] += 1; // 1 Sub vom Team in der Zone gezählt
                }
            }
        }
        // Alleiniges Team mit Subs in der Zone
        if (team_subs.size() == 1) team = team_subs.begin()->first;
        else team = 0; // Keine Subs hier oder Subs von verschiedenen Teams
    }
}
