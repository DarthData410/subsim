#pragma once

#include "../src/sim/welt.hpp"
#include <doctest.h>

class Test_Welt {

TEST_CASE_CLASS("welt") {
    Welt welt(0);
    SUBCASE("teams") {
        REQUIRE(welt.get_team_anzahl() == 2);
        REQUIRE_NOTHROW(welt.get_team(1));
        REQUIRE_NOTHROW(welt.get_team(2));
    }
    SUBCASE("zonen") {
        REQUIRE(welt.get_zonen().size() > 0);
        for (const auto& zone : welt.get_zonen()) REQUIRE(zone.get_team() == 0);
    }
    SUBCASE("gegner nimmt zone ein") {
        REQUIRE(welt.get_objekte().size() == 0);
        const uint8_t team = 1;
        welt.add_new_sub(team, true);
        welt.add_new_sub(team, true);
        REQUIRE(welt.get_objekte().size() == 2);
        bool zone_eingenommen = false;
        for (unsigned i = 0; i < 100'000; ++i) { // Anzahl ticks
            welt.tick(10); // s pro tick
            for (const auto& zone : welt.get_zonen()) if (zone.get_team() == team) { zone_eingenommen = true; break; }
        }
        REQUIRE(zone_eingenommen == true);
        REQUIRE(welt.get_team(team).get_punkte() > 0);
    }
    SUBCASE("sonar detektion") {

    }
    SUBCASE("torpedo abschuss und treffer") {
        // 2 zueinander feindliche Subs erzeugen
        REQUIRE(welt.get_objekte().size() == 0); // Welt leer
        auto sub1_id = welt.add_new_sub(1, false)->get_id();
        auto sub2_id = welt.add_new_sub(2, false)->get_id();
        REQUIRE(welt.get_objekte().size() == 2);
        Sub* sub1;
        Sub* sub2;
        REQUIRE_NOTHROW(sub1 = dynamic_cast<Sub*>(welt.get_objekte().at(sub1_id)));
        REQUIRE_NOTHROW(sub2 = dynamic_cast<Sub*>(welt.get_objekte().at(sub2_id)));

        // sub2 nähert sich auf zieldistanz
        auto distanz = [&]() { return Physik::distanz_xy(sub1->get_pos(), sub2->get_pos()); };
        const double zieldistanz = 100.0;
        sub2->set_target_v(1.0);
        sub2->set_target_pos(sub1->get_pos().x() + zieldistanz, sub1->get_pos().y());
        for (unsigned i = 0; i < 100'000; ++i) welt.tick(1); // Zeit um sich zu naehern
        REQUIRE(distanz() < zieldistanz * 1.5); // Distanz + Toleranz voneinander entfernt (kann bei >1 ticks ungenauer werden)
        REQUIRE(sub1->get_speed() == doctest::Approx(0.f));
        REQUIRE(sub2->get_speed() == doctest::Approx(0.f));

        // sub1 Sonar sieht sub2

        // sub1 feuert Torpedo auf sub2
        REQUIRE(sub1->get_torpedos().empty() == false); // Torpedotypen existieren
        const auto& torpedotyp = sub1->get_torpedos().begin()->first;
        REQUIRE(sub1->get_torpedos().at(torpedotyp) > 0); // torpedotyp hat Munition
        REQUIRE(torpedotyp.get_range() * 0.5 >= distanz()); // Ziel in halber Reichweite (zur Sicherheit)
        Torpedo torpedo(torpedotyp);
        torpedo.set_target_bearing(Physik::kurs(sub1->get_pos(), sub2->get_pos()));
        torpedo.set_target_depth(sub2->get_pos().z());

        welt.shoot_torpedo(sub1, torpedo);
        for (unsigned i = 0; i < 100'000; ++i) welt.tick(1); // Zeit für Torpedo für was auch immer
        REQUIRE(welt.get_objekte().size() == 2); // Übrig: 2 Subs
    }
}

};
