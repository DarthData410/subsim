#include <log.hpp>
#include "kommando.hpp"
#include "../welt.hpp"
#include "../game/objects/sub.hpp"

void Kommando::apply(Welt* welt) {
    if (welt->objekte.count(sub_id) == 0) {
        Log::debug() << "Kommando::apply sub_id not found: " << sub_id << Log::endl;
        return;
    }
    Sub* sub = (Sub*) welt->objekte[sub_id];
    switch (typ) {
        case STOP:
            sub->stop();
            break;
        case MOTOR_LINEAR:
            sub->set_target_v(as<float>());
            break;
        case MOTOR_ROT:
            sub->set_target_rudder(as<float>());
            break;
        case MOTOR_TAUCH:
            // TODO
            break;
        case AUTO_POS: {
            const auto& pos = as<std::tuple<float, float>>();
            sub->set_target_pos(std::get<0>(pos), std::get<1>(pos));
        }   break;
        case AUTO_KURS:
            sub->set_target_bearing(as<float>());
            break;
        case AUTO_TIEFE:
            // TODO
            break;
        case TORP_LAUNCH:
            if (Torpedo* t = new Torpedo(as<Torpedo>()); sub->shoot(t->get_name())) {
                if (welt->objekte.count(t->get_id())) Log::err() << "Error: Objekt duplicate with ID: " << t->get_id() << '\n';
                else welt->objekte[t->get_id()] = t; // Torpedo in die Welt gesetzt
            } else Log::err() << "Error: Torpedo could not be launched. " << t->get_name() << '\n';
            break;
        case TIMELAPSE:
            welt->timelapse = as<float>();
            Log::debug() << "Welt hat jetzt Timelapse = " << welt->timelapse << '\n';
            break;
        default:
            Log::err() << "Kommando::apply typ not implemented: " << (unsigned)typ << '\n';
            break;
    }
}
