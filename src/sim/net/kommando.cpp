#include <log.hpp>
#include "kommando.hpp"
#include "../welt.hpp"

void Kommando::apply(Welt* welt) {
    if (welt->objekte.count(sub_id) == 0) {
        Log::debug() << "Kommando::apply sub_id not found: " << sub_id << Log::endl;
        return;
    }
    Sub* sub = (Sub*) welt->objekte[sub_id];
    switch (typ) {
        case STOP: sub->stop();
            break;
        case MOTOR_LINEAR: sub->set_target_v(as<float>());
            break;
        case MOTOR_ROT: sub->set_target_rudder(as<float>());
            break;
        case MOTOR_TAUCH: // TODO
            break;
        case AUTO_POS: {
            const auto& pos = as<std::tuple<float, float>>();
            sub->set_target_pos(std::get<0>(pos), std::get<1>(pos));
            break;
        }
        case AUTO_KURS: sub->set_target_bearing(as<float>());
            break;
        case AUTO_TIEFE: // TODO
            break;
    }
}
