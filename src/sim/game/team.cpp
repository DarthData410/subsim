#include "team.hpp"

#include <zufall.hpp>

Team::Team(uint8_t id) : id(id), punkte(0) {
    // Stock Sub
    const Motor m1_linear(4, 0.25);
    const Motor m1_rot(1, 0.1);
    const Motor m1_tauch(0.5, 0.1);
    Sub beginner_sub("Type 1", {0,0,0}, m1_linear, m1_rot, m1_tauch);
    beginner_sub.team = Team::id;
    beginner_sub.noise = 0.95f;

    // Stock Passiver Sonar // TODO: Test/Fix blindspots
    const std::vector<std::tuple<float,float>> blindspots1 = {{180, 170}}; // 10° links/rechts vorn sichtbar
    const std::vector<std::tuple<float,float>> blindspots2 = {{180,  90}}; // 90° links/rechts vorn sichtbar (=vordere Hälfte)
    const Sonar_Aktiv  sonar_aktiv_t("TAS-1", Sonar::Groesse::KLEIN, 15.f, 100, 10'000, 10.f, {}); // für Torpedos
    const Sonar_Aktiv  sonar_aktiv_s("SAS-1", Sonar::Groesse::GROSS, 15.f, 100, 10'000, 10.f, {}); // für Subs
    const Sonar_Passiv sonar_passiv_t("TPS-1", Sonar::Groesse::KLEIN, 0.4f, 15.f, {}); // für Torpedos
    const Sonar_Passiv sonar_passiv_s("SPS-1", Sonar::Groesse::GROSS, 0.4f, 15.f, {}); // für Subs
    //const Sonar_Passiv sonar_passiv_s(0.4f, 20.f, blindspots3);
    beginner_sub.sonars_active.emplace_back(sonar_aktiv_s);
    beginner_sub.sonars_passive.emplace_back(sonar_passiv_s);

    // Stock Decoys
    const Decoy decoy1(Motor(0,0), Motor(0,0), Motor(0,0),
                       "Decoy Model 1", 0.75f, 30.f);
    beginner_sub.decoys[decoy1] = 16;

    // Stock Torpedo
    const Torpedo torpedo1(Motor(10, 1),
                           Motor(20, 1),
                           Motor(2, 0.25),
                           "T1A",
                           10'000,
                           Explosion(30.f, 50, 10),
                           sonar_aktiv_t,
                           sonar_passiv_t);
    beginner_sub.torpedos[torpedo1] = 6;

    // Designs speichern
    sub_designs.push_back(beginner_sub);
    torp_designs.push_back(torpedo1);
    sonars_a.push_back(sonar_aktiv_s);
    sonars_a.push_back(sonar_aktiv_t);
    sonars_p.push_back(sonar_passiv_s);
    sonars_p.push_back(sonar_passiv_t);
}

const Sub& Team::get_new_sub() {
    return Zufall::wahl(sub_designs);
}
