#pragma once

#include "../src/sim/welt.hpp"
#include <doctest.h>

class Test_Welt {

TEST_CASE_CLASS("welt") {
    Welt welt(0);
    SUBCASE("teams") {
        CHECK(welt.get_team_anzahl() == 2);
        CHECK_NOTHROW(welt.get_team(1));
        CHECK_NOTHROW(welt.get_team(2));
    }
    SUBCASE("zonen") {
        CHECK(welt.get_zonen().size() > 0);
        for (const auto& zone : welt.get_zonen()) CHECK(zone.get_team() == 0);
    }
    SUBCASE("gegner nimmt zone ein") {
        CHECK(welt.get_objekte().size() == 0);
        const uint8_t team = 1;
        welt.add_new_sub(team, true);
        welt.add_new_sub(team, true);
        CHECK(welt.get_objekte().size() == 2);
        bool zone_eingenommen = false;
        for (unsigned i = 0; i < 100'000; ++i) { // Anzahl ticks
            welt.tick(10); // s pro tick
            for (const auto& zone : welt.get_zonen()) if (zone.get_team() == team) { zone_eingenommen = true; break; }
        }
        CHECK(zone_eingenommen == true);
        CHECK(welt.get_team(team).get_punkte() > 0);
    }
    SUBCASE("szenario torpedo abschuss und treffer") {
        // 2 zueinander feindliche Subs erzeugen
        CHECK(welt.get_objekte().size() == 0); // Welt leer
        auto sub1_id = welt.add_new_sub(1, false)->get_id();
        auto sub2_id = welt.add_new_sub(2, false)->get_id();
        CHECK(welt.get_objekte().size() == 2);
        Sub* sub1;
        Sub* sub2;
        CHECK_NOTHROW(sub1 = dynamic_cast<Sub*>(welt.get_objekte().at(sub1_id).get()));
        CHECK_NOTHROW(sub2 = dynamic_cast<Sub*>(welt.get_objekte().at(sub2_id).get()));

        // sub2 nähert sich auf zieldistanz
        auto distanz = [&]() { return Physik::distanz_xy(sub1->get_pos(), sub2->get_pos()); };
        const double zieldistanz = 2000.0;
        sub2->set_target_v(1.0);
        sub2->set_target_pos(sub1->get_pos().x() + zieldistanz, sub1->get_pos().y());
        for (unsigned i = 0; i < 100'000; ++i) welt.tick(1); // Zeit um sich zu naehern
        CHECK(distanz() < zieldistanz * 1.1); // Distanz + Toleranz voneinander entfernt (kann bei großen ticks ungenauer werden)
        CHECK(sub2->get_speed() == doctest::Approx(0.f));

        // sub2 im Kreis herum im Uhrzeigersinn fahren lassen
        sub2->set_target_rudder(1.0);
        sub2->set_target_v(1.0);
        for (unsigned i = 0; i < 1000; ++i) welt.tick(1);

        CHECK(sub1->get_speed()         == doctest::Approx(0));
        CHECK(sub1->get_speed_relativ() == 0.0);
        CHECK(sub2->get_speed()         == sub2->get_speed_max());
        CHECK(sub2->get_speed_relativ() == 1.0);
        const auto& sonar = sub1->get_sonars().front();
        const auto& detektionen = sonar.get_detections();

        SUBCASE("sonar detektion") {
            // sub1 Sonar sieht sub2
            const auto kurs_relativ = Physik::kurs_relativ(sub1, sub2);
            CAPTURE(sub1->get_bearing());
            CAPTURE(Physik::kurs(sub1->get_pos(), sub2->get_pos()));
            CAPTURE(kurs_relativ);
            CHECK(sonar.is_in_blindspot(Physik::kurs_relativ(sub1, sub2)) == false);
            CHECK(detektionen.size() == 1);
            CHECK(detektionen.front().bearing == doctest::Approx(Physik::round(kurs_relativ, sonar.get_resolution())));
        }

        SUBCASE("torpedo abschuss") {
            // sub1 stellt Torpedo ein auf sub2
            CHECK(sub1->get_torpedos().empty() == false); // Torpedotypen existieren
            const auto& torpedotyp = sub1->get_torpedos().begin()->first;
            CHECK(sub1->get_torpedos().at(torpedotyp) > 0); // torpedotyp hat Munition
            CHECK(torpedotyp.get_range() * 0.5 >= distanz()); // Ziel in halber Reichweite (zur Sicherheit)
            Torpedo torpedo(torpedotyp);
            torpedo.set_target_bearing(detektionen.front().bearing);
            torpedo.set_target_depth(sub2->get_pos().z());
            torpedo.set_distance_to_activate(250);
            torpedo.set_distance_to_explode(50);
            CHECK(torpedo.get_distance_to_activate() > torpedo.get_distance_to_explode());

            // sub1 feuert Torpedo auf sub2
            welt.add_torpedo(sub1, torpedo);
            CHECK(welt.get_objekte().size() == 3);

            // Treffer + Explosion erwartet
            bool war_explosion = false;
            for (unsigned i = 0; i < 100'000; ++i) {
                welt.tick(0.1); // Zeit für Torpedo
                if (!war_explosion) for (const auto& o: welt.get_objekte()) if (o.second->get_typ() == Objekt::Typ::EXPLOSION) war_explosion = true;
            }
            CHECK(war_explosion);
            CHECK(welt.get_objekte().size() == 1); // Übrig: 1 Sub
        }
    }
}

};
