#include "sub_ai.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <zufall.hpp>

Sub_AI::Sub_AI(const Sub& sub) : Sub(sub){

}

bool Sub_AI::tick(Welt* welt, float s) {
    if (!Sub::tick(welt, s)) return false; // Lebt nicht mehr

    // Nicht jeden tick() nachdenken
    timer += s;
    if (timer < 1000.f) return true;
    timer = 0;

    // Nichts zu tun -> Zone einnehmen / bewachen
    if (status == DONE) {
        Log::debug() << "Sub_AI " << id << " looking for new job" << Log::endl;
        // Zone finden und als Ziel setzen
        const auto it = std::find_if(welt->zonen.begin(), welt->zonen.end(), [this](const Zone& zone) {
            return zone.get_team() != this->get_team();
        });
        if (it != welt->zonen.end()) ziel = Vektor(std::get<0>(it->get_pos()), std::get<1>(it->get_pos()), DEPTH_TRAVEL);
        else {
            const Zone& zone = welt->zonen[Zufall::ui(0, welt->zonen.size()-1)];
            ziel = Vektor(std::get<0>(zone.get_pos()),  std::get<1>(zone.get_pos()), DEPTH_TRAVEL);
        }
        add_status(TRAVEL);
    }

    // Zum Ziel bewegen
    if (hat_status(TRAVEL)) {
        set_target_pos(ziel.x(), ziel.y());
        if (hat_status(HIDE)) stop();
        else set_target_v(SPEED_TRAVEL);

        // Ziel erreicht?
        if (Physik::distanz(pos.x(), pos.y(), ziel.x(), ziel.y()) < 1000.f) {
            Log::debug() << "Sub_AI " << id << " target reached" << Log::endl;
            remove_status(TRAVEL);
            add_status(SEARCH);
            stop();
        }
    }
    return true;
}