#include "sub_ai.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <zufall.hpp>

Sub_AI::Sub_AI(const Sub& sub) : Sub(sub), timer(0), timer_next_action(0) {

}

bool Sub_AI::tick(Welt* welt, float s) {
    if (const bool alive = Sub::tick(welt, s); !alive) return false; // Lebt nicht mehr

    // Nicht jeden tick() nachdenken
    timer += s;
    if (timer < timer_next_action) return true;
    timer = 0;

    // Kontakte sammeln
    std::vector<const Detektion*> d_active; // Nur AS
    std::vector<const Detektion*> d_passive; // Nur PS
    auto is_gegner = [&](const Detektion& d) {
        const Objekt* obj = welt->get_objekt_or_null(d.objekt_id);
        return obj && (obj->get_typ() == Objekt::Typ::SUB || obj->get_typ() == Objekt::Typ::SUB_AI) && obj->get_team() != this->get_team();
    };
    for (const auto& as: get_sonars_active())   for (const auto& d: as.get_detektionen()) if (is_gegner(d)) d_active.push_back(&d);
    for (const auto& ps: get_sonars_passive()) for (const auto& d: ps.get_detektionen()) if (is_gegner(d)) d_passive.push_back(&d);
    std::vector<const Detektion*> detektionen(d_active); // AS + PS
    detektionen.insert(detektionen.end(), d_passive.begin(), d_passive.end());

    // Kontakte?
    if (!detektionen.empty()) {
        // AS Detektion? Ja - Hin/Angreifen; Nein - Suchen
        if (d_active.empty() && !d_passive.empty()) add_status(SEARCH); // !AS, aber PS -> Suchen
        else if (!d_active.empty()) remove_status(SEARCH); // AS Kontakt vorhanden -> nicht mehr suchen

        // Mit AS suchen?
        if (d_active.empty() && !sonars_active.empty() && !hat_status(HIDE) && hat_status(SEARCH)) {
            Log::debug() << "Sub_AI::tick " << id << " trying single Ping" << Log::endl;
            auto& as = sonars_active[0];
            as.set_ping_intervall(300); // zu häufiges Pingen verhindern
            as.set_mode(Sonar_Aktiv::Mode::SINGLE);
        }

        // Zielauswahl
        const auto torpedo_range = this->get_max_reichweite_torpedo(true);
        const Detektion* angriffsziel = detektionen[0];
        for (const auto& d : detektionen) {
            if (angriffsziel->range.has_value() && d->range.has_value() && d->range < angriffsziel->range) angriffsziel = d;
        }

        // Sich weiter nähern?
        if (!angriffsziel->range.has_value() || (angriffsziel->range.has_value() && angriffsziel->range.value() > torpedo_range)) {
            const auto [ziel_x, ziel_y] = Physik::get_punkt(this->pos.x(), this->pos.y(), angriffsziel->bearing, 1'000);
            this->ziel.x(ziel_x); this->ziel.y(ziel_y);
            remove_status(SEARCH);
        }
        else { // Angriff!
            
        }
    }

    // Nichts zu tun -> Zone einnehmen / bewachen
    if (status == DONE) {
        Log::debug() << "Sub_AI::tick " << id << " looking for new job" << Log::endl;

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
        timer_next_action = AI_THINK_INTERVALL;
    }

    // TODO:
    // if hasEnemyPassiveContact
    //      -> ATTACK,
    //      then hide (?)
    //      then wait

    // TODO
    // if isUnderAttack
    //      -> EVADE
    //          USE DECOY
    //          then wait
    //          then retaliate -> ATTACK (?)

    // TODO
    // if noContacts
    //      clear combat statusses

    // TODO
    if (hat_status(HIDE)) {
        stop();
        set_target_depth(-500);
    }

    // Zum Ziel bewegen
    if (hat_status(TRAVEL)) {
        // Ziel erreicht?
        if (Physik::distanz(pos.x(), pos.y(), ziel.x(), ziel.y()) <= TARGET_DISTANCE) {
            Log::debug() << "Sub_AI " << id << " target reached" << Log::endl;
            remove_status(TRAVEL);
            add_status(SEARCH);
            stop();
        }
        set_target_pos(ziel.x(), ziel.y());
        set_target_depth(DEPTH_TRAVEL);
        set_target_v(SPEED_TRAVEL);
    }
    return true;
}

dist_t Sub_AI::get_zielentfernung() const {
    return Physik::distanz_xy(this->pos, this->ziel);
}
