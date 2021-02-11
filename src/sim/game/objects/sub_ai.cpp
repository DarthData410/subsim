#include "sub_ai.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <zufall.hpp>

Sub_AI::Sub_AI(const Sub& sub) : Sub(sub){

}

bool Sub_AI::tick(Welt* welt, float s) {
    if (const bool alive = Sub::tick(welt, s); !alive) return false; // Lebt nicht mehr
    if (timer.getMilliseconds() < 1000.f) return true; // Nicht jeden tick() nachdenken

    // Konfig
    const float SPEED_TRAVEL = 0.75f;
    const float DEPTH_TRAVEL = -50.f;

    // Nichts zu tun -> Zone einnehmen / bewachen
    if (status == DONE) {
        Log::debug() << "Sub_AI " << id << " looking for new job" << Log::endl;
        // Zone finden und als Ziel setzen
        const auto it = std::find_if(welt->zonen.begin(), welt->zonen.end(), [this](const Zone& zone) {
            return zone.get_team() != this->get_team();
        });
        if (it != welt->zonen.end()) ziel = Ogre::Vector3(std::get<0>(it->get_pos()), DEPTH_TRAVEL, std::get<1>(it->get_pos()));
        else {
            const Zone& zone = welt->zonen[Zufall::ui(0, welt->zonen.size()-1)];
            ziel = Ogre::Vector3(std::get<0>(zone.get_pos()), DEPTH_TRAVEL, std::get<1>(zone.get_pos()));
        }
        add_status(TRAVEL);
    }

    // Zum Ziel bewegen
    if (hat_status(TRAVEL)) {
        set_target_pos(ziel.x, ziel.z);
        if (hat_status(HIDE)) stop();
        else set_target_v(SPEED_TRAVEL);

        // Ziel erreicht?
        if (Physik::distanz(pos.x, pos.z, ziel.z, ziel.z) < 1000.f) {
            Log::debug() << "Sub_AI " << id << " target reached" << Log::endl;
            remove_status(TRAVEL);
            add_status(SEARCH);
            stop();
        }
    }
    timer.reset();
    return true;
}
