#include <log.hpp>
#include "kommando.hpp"
#include "../welt.hpp"
#include "../game/objects/sub.hpp"

void Kommando::apply(Welt* welt) {
    Sub* sub = nullptr;
    if (welt->objekte.count(sub_id)) sub = (Sub*) welt->objekte[sub_id];
    else Log::debug() << "Kommando::apply sub_id not found: " << sub_id << Log::endl; // für TIMELAPSE okay
    Log::debug() << "Kommando::apply sub_id=" << sub_id << " Kommando=" << (int)typ << Log::endl; // für TIMELAPSE okay

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
        case TORP_LAUNCH: {
            const Torpedo& t = as<Torpedo>();
            welt->shoot_torpedo(sub, t);
        }   break;
        case TIMELAPSE:
            welt->timelapse = as<float>();
            Log::debug() << "Welt hat jetzt Timelapse = " << welt->timelapse << '\n';
            break;
        default:
            Log::err() << "Kommando::apply typ not implemented: " << (unsigned)typ << '\n';
            break;
    }
}
