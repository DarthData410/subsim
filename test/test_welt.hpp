#pragma once

#include "../src/sim/welt.hpp"
#include <doctest.h>
#include <log.hpp>

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
    SUBCASE("sonar passiv keine detektion") {
        CHECK(welt.get_objekte().size() == 0);
        const uint8_t team = 1;
        for (unsigned i = 0; i < 10; ++i) welt.add_new_sub(team, false);
        CHECK(welt.get_objekte().size() == 10);
        for (unsigned i = 0; i < 1'200; ++i) welt.tick(0.1); // 2 min warten
        for (const auto& [id, o]  : welt.get_objekte()) {
            CHECK(o->get_speed() == doctest::Approx(0));
            Sub* sub = dynamic_cast<Sub*>(o.get());
            CHECK(sub != nullptr);
            CHECK(sub->get_sonars_passive().at(0).get_detektionen().size() == 0);
        }
    }
    SUBCASE("szenario test 1vs1") {
        // 2 zueinander feindliche Subs erzeugen
        CHECK(welt.get_objekte().size() == 0); // Welt leer
        auto sub1_id = welt.add_new_sub(1, false)->get_id();
        auto sub2_id = welt.add_new_sub(2, false)->get_id();
        CHECK(welt.get_objekte().size() == 2);
        Sub* sub1;
        Sub* sub2;
        CHECK_NOTHROW(sub1 = dynamic_cast<Sub*>(welt.get_objekte().at(sub1_id).get()));
        CHECK_NOTHROW(sub2 = dynamic_cast<Sub*>(welt.get_objekte().at(sub2_id).get()));
        CHECK(sub1 != nullptr);
        CHECK(sub2 != nullptr);
        CHECK(sub1->get_typ() == Objekt::Typ::SUB);
        CHECK(sub2->get_typ() == Objekt::Typ::SUB);

        // AS einschalten
        sub1->sonars_active.begin()->set_mode(Sonar_Aktiv::Mode::ON);
        sub2->sonars_active.begin()->set_mode(Sonar_Aktiv::Mode::ON);

        // sub2 nähert sich auf zieldistanz
        auto distanz = [&]() { return Physik::distanz_xy(sub1->get_pos(), sub2->get_pos()); };
        const double zieldistanz = 2000.0;
        sub2->set_target_v(1.0);
        sub2->set_target_pos(sub1->get_pos().x() + zieldistanz, sub1->get_pos().y());
        for (unsigned i = 0; i < 100'000; ++i) welt.tick(1); // Zeit um sich zu naehern
        CHECK(distanz() < zieldistanz * 1.1); // Distanz + Toleranz voneinander entfernt (kann bei großen ticks ungenauer werden)
        CHECK(sub2->get_speed() == doctest::Approx(0.f));

        SUBCASE("sonar_aktiv detektion") {
            CHECK(sub1->sonars_active.begin()->get_detektionen().size() == 1);
            CHECK(sub2->sonars_active.begin()->get_detektionen().size() == 1);
        }
        sub1->sonars_active.begin()->set_mode(Sonar_Aktiv::Mode::OFF);
        sub2->sonars_active.begin()->set_mode(Sonar_Aktiv::Mode::OFF);

        // sub2 im Kreis herum im Uhrzeigersinn fahren lassen
        sub2->set_target_rudder(1.0);
        sub2->set_target_v(1.0);
        for (unsigned i = 0; i < 1000; ++i) welt.tick(1);

        CHECK(sub1->get_speed()         == doctest::Approx(0));
        CHECK(sub1->get_speed_relativ() == 0.0);
        CHECK(sub2->get_speed()         == sub2->get_speed_max());
        CHECK(sub2->get_speed_relativ() == 1.0);
        const auto& sonar_passiv = sub1->get_sonars_passive().front();
        const auto& detektionen = sonar_passiv.get_detektionen();

        SUBCASE("sonar_passiv passiv detektion") {
            // sub1 Sonar sieht sub2
            const auto kurs_relativ = Physik::kurs_relativ(sub1, sub2);
            CAPTURE(sub1->get_bearing());
            CAPTURE(Physik::kurs(sub1->get_pos(), sub2->get_pos()));
            CAPTURE(kurs_relativ);
            CHECK(sonar_passiv.is_in_toter_winkel(Physik::kurs_relativ(sub1, sub2)) == false);
            REQUIRE(detektionen.size() == 1);
            CHECK(detektionen.front().bearing == doctest::Approx(Physik::round(kurs_relativ, sonar_passiv.get_aufloesung())));
        }

        SUBCASE("torpedo abschuss") {
            // sub1 stellt Torpedo ein auf sub2
            CHECK(sub1->get_torpedos().empty() == false); // Torpedotypen existieren
            const auto& torpedotyp = sub1->get_torpedos().begin()->first;
            CHECK(sub1->get_torpedos().at(torpedotyp) > 0); // torpedotyp hat Munition
            CHECK(torpedotyp.get_range() * 0.5 >= distanz()); // Ziel in halber Reichweite (zur Sicherheit)

            // sub1 feuert Torpedo auf sub2
            static std::set<oid_t> ping_ids;
            auto shoot_new_torpedo = [&](
                    Welt& welt,
                    Sub* sub1, Sub* sub2,
                    bool& war_explosion,
                    double& min_distance) {
                // Treffer + Explosion erwartet
                Torpedo torpedo;
                /// Torpedo mit Ammo auswählen + einstellen
                bool torpedo_gefunden = false;
                for (const auto& torp_paar : sub1->get_torpedos()) if (torp_paar.second > 0) {
                    torpedo_gefunden = true;
                    torpedo = torp_paar.first;
                    break;
                }
                if (!torpedo_gefunden) return false;
                torpedo.set_target_bearing(sub1->get_sonars_passive().front().get_detektionen().front().bearing);
                torpedo.set_target_depth(sub2->get_pos().z());
                torpedo.set_distance_to_activate(250);
                torpedo.set_distance_to_explode(50);
                CHECK(torpedo.get_distance_to_activate() > torpedo.get_distance_to_explode());
                int ammo_vorher = -1;
                REQUIRE_NOTHROW(ammo_vorher = sub1->get_torpedos().at(torpedo));
                welt.add_torpedo(sub1, torpedo);
                CHECK(sub1->get_torpedos().at(torpedo) == ammo_vorher - 1); // 1 Torpedo weniger?
                for (unsigned i = 0; i < 100'000; ++i) {
                    welt.tick(0.1); // Zeit für Torpedo
                    if (!war_explosion) for (const auto& o: welt.get_objekte()) {
                        if (o.second->get_typ() == Objekt::Typ::TORPEDO && sub2) {
                            const double d = Physik::distanz_xyz(o.second->get_pos(), sub2->get_pos());
                            min_distance = std::min(min_distance, d);
                        }
                        else if (o.second->get_typ() == Objekt::Typ::EXPLOSION) war_explosion = true;
                        else if (o.second->get_typ() == Objekt::Typ::PING) ping_ids.insert(o.second->get_id());
                    }
                }
                return true;
            };

            /// Auf Sub2 schießen, bis weg oder Ammo leer
            bool war_explosion = false;
            double min_distance = 999999;
            unsigned anzahl_versuche = 0;
            while (welt.get_objekt_or_null(sub1_id) && welt.get_objekt_or_null(sub2_id)) {
                anzahl_versuche++;
                if (!shoot_new_torpedo(welt, sub1, sub2, war_explosion, min_distance)) break; // keine Ammo mehr
            }
            std::cout << "Test_welt Abschussversuche=" << anzahl_versuche << '\n';
            std::cout << "Test_welt Sonar Pings=" << ping_ids.size() << '\n';
            CAPTURE(min_distance);
            CHECK(ping_ids.size() > 0);
            CHECK(war_explosion == true);
            CHECK(welt.get_objekte().size() == 1); // Übrig: 1 Sub
            CHECK(welt.abschuesse.size() == 1); // 1 Statistik vorhanden
            if (!welt.abschuesse.empty()) Log::out() << welt.abschuesse.front().get_as_text() << '\n';
        }
    }
}

};
