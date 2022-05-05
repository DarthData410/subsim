#pragma once

#include "../src/sim/welt.hpp"
#include "../src/sim/game/karte.hpp"
#include <doctest.h>
#include <log.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SimplexNoise.h>
#include <zufall.hpp>

class Test_Welt {

TEST_CASE_CLASS("welt") {
    Welt welt(0);
    SUBCASE("teams") {
        CHECK(welt.get_team_anzahl() == 2);
        REQUIRE_NOTHROW(welt.get_team(1));
        REQUIRE_NOTHROW(welt.get_team(2));
    }
    SUBCASE("zonen") {
        REQUIRE(welt.get_zonen().size() > 0);
        for (const auto& zone : welt.get_zonen()) CHECK(zone.get_team() == 0);
    }
    SUBCASE("Objekt_Steuerbar Pathfinding: auto_rudder, auto_depth") {
        const auto sub_id = welt.add_new_sub(1, false)->get_id();
        REQUIRE(welt.get_objektanzahl(Objekt::Typ::SUB) == 1);
        Sub* sub = nullptr;
        CHECK_NOTHROW(sub = dynamic_cast<Sub*>(welt.get_objekte().at(sub_id).get()));

        sub->set_target_v(sub->get_speed_max());
        for (unsigned i = 0; i < 10'000; ++i) welt.tick(0.1f);
        REQUIRE(sub->get_speed_relativ() == doctest::Approx(1.0)); // Höchstgeschwindigkeit erreicht

        sub->set_target_bearing(160); // Drehen
        for (unsigned i = 0; i < 10'000; ++i) welt.tick(0.1f);
        REQUIRE(Physik::round(sub->get_bearing(), 10.f) == doctest::Approx(160.0f));

        sub->set_target_depth(-75.f); // Untertauchen
        for (unsigned i = 0; i < 10'000; ++i) welt.tick(0.1f);
        REQUIRE(Physik::round(sub->get_pos().z(), 5.0) == doctest::Approx(-75.0));

        sub->set_target_depth(-150.f); // Untertauchen
        for (unsigned i = 0; i < 10'000; ++i) welt.tick(0.1f);
        REQUIRE(Physik::round(sub->get_pos().z(), 5.0) == doctest::Approx(-150.0));

        sub->set_target_depth(-25.f); // Auftauchen
        for (unsigned i = 0; i < 10'000; ++i) welt.tick(0.1f);
        REQUIRE(Physik::round(sub->get_pos().z(), 5.0) == doctest::Approx(-25.0));
    }
    SUBCASE("gegner nimmt zone ein") {
        CHECK(welt.get_objekte().size() == 0);
        const uint8_t team = 1;
        welt.add_new_sub(team, true);
        welt.add_new_sub(team, true);
        CHECK(welt.get_objekte().size() == 2);
        REQUIRE(welt.get_objektanzahl(Objekt::Typ::SUB_AI) == 2);
        bool zone_eingenommen = false;
        for (unsigned i = 0; i < 100'000; ++i) { // Anzahl ticks
            welt.tick(10); // s pro tick
            for (const auto& zone: welt.get_zonen()) if (zone.get_team() == team) {
                zone_eingenommen = true;
                break;
            }
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
        Sub* sub1 = nullptr;
        Sub* sub2 = nullptr;
        REQUIRE_NOTHROW(sub1 = dynamic_cast<Sub*>(welt.get_objekte().at(sub1_id).get()));
        REQUIRE_NOTHROW(sub2 = dynamic_cast<Sub*>(welt.get_objekte().at(sub2_id).get()));
        REQUIRE(sub1 != nullptr);
        REQUIRE(sub2 != nullptr);
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
            CHECK(detektionen.front().bearing ==
                  doctest::Approx(Physik::round(kurs_relativ, sonar_passiv.get_aufloesung())));
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
                welt.add_torpedo(sub1, &torpedo);
                CHECK(sub1->get_torpedos().at(torpedo) == ammo_vorher - 1); // 1 Torpedo weniger?
                for (unsigned i = 0; i < 100'000; ++i) {
                    welt.tick(0.1); // Zeit für Torpedo
                    if (!war_explosion) for (const auto& o: welt.get_objekte()) {
                        if (o.second->get_typ() == Objekt::Typ::TORPEDO && sub2) {
                            const double d = Physik::distanz_xyz(o.second->get_pos(), sub2->get_pos());
                            min_distance = std::min(min_distance, d);
                        } else if (o.second->get_typ() == Objekt::Typ::EXPLOSION) war_explosion = true;
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
    SUBCASE("evasion durch decoy") {
        const auto sub1_id = welt.add_new_sub(1, false)->get_id();
        const auto sub2_id = welt.add_new_sub(2, false)->get_id();
        Sub* sub1 = nullptr;
        Sub* sub2 = nullptr;
        CHECK_NOTHROW(sub1 = dynamic_cast<Sub*>(welt.get_objekte().at(sub1_id).get()));
        CHECK_NOTHROW(sub2 = dynamic_cast<Sub*>(welt.get_objekte().at(sub2_id).get()));
        REQUIRE(sub1 != nullptr);
        REQUIRE(sub2 != nullptr);
        sub1->pos = {0, 500, 0}; // oberhalb
        sub1->kurs = 179;         // nach unten schauend
        sub2->pos = {0,0,0};   // unterhalb, leicht links zu Beginn
        sub2->kurs = 90;          // nach rechts schauend
        sub2->set_target_v(sub2->get_speed_max()); // Volle Fahrt voraus
        Torpedo torpedo;
        /// Torpedo mit Ammo auswählen + einstellen
        bool torpedo_gefunden = false;
        for (const auto& torp_paar : sub1->get_torpedos()) if (torp_paar.second > 0) {
                torpedo_gefunden = true;
                torpedo = torp_paar.first;
                break;
        }
        REQUIRE(torpedo_gefunden);
        torpedo.set_target_bearing(170);
        torpedo.set_target_depth(sub2->get_pos().z());
        torpedo.set_distance_to_activate(250);
        torpedo.set_distance_to_explode(50);
        welt.add_torpedo(sub1, &torpedo);
        welt.add_decoy(sub2, &(sub2->decoys.begin()->first));
        //sub2->schaeden.insert(Objekt_Steuerbar::Schaden::ZERSTOERT);
        for (unsigned i = 0; i < 100'000; ++i) {
            welt.tick(0.1); // Zeit zum Simulieren
            if (welt.get_objektanzahl(Objekt::Typ::TORPEDO) && // neuen Decoy starten?
                sub2->decoys.begin()->second &&
                welt.get_objektanzahl(Objekt::Typ::DECOY) == 0) {
                welt.add_decoy(sub2, &(sub2->decoys.begin()->first));
            }
        }
        REQUIRE(welt.get_objekte().size() <= 2); // 1 oder 2 Subs übrig
    }
}
TEST_CASE_CLASS("karte" * doctest::timeout(30)) {
    SUBCASE("terrain generierung") {
        Karte karte;
        const auto karte_raw = karte.get_image(100, 100);
        CHECK_NOTHROW(karte_raw->saveToFile("temp_karte.png"));
        float min = 0.5f; // noise min
        float max = 0.5f; // noise max
        float min_h = 0;  // höhe min
        float max_h = 0;  // höhe max
        for (int x = -1000; x <= 1000; ++x) for (int y = -1000; y <= 1000; ++y) {
            const auto val = karte.get_raw_at(x*100,y*100);
            const auto height = karte.get_height_at(x*100,y*100);
            min = std::min(min, val);
            max = std::max(max, val);
            min_h = std::min(min_h, height);
            max_h = std::max(max_h, height);
        }
        REQUIRE(min >= 0.f);
        REQUIRE(max <= 1.f);
        REQUIRE(min_h  < 0);
        REQUIRE(max_h >= 0);
    }
    SUBCASE("terrain passierbarkeit") {
        Karte karte;
        for (unsigned i = 0; i < 100; ++i) {
            // Zufällige Position
            const float x = Zufall::f(-100000.f, 100000.f);
            const float y = Zufall::f(-100000.f, 100000.f);
            {   // Mindestens
                const auto [px, py] = karte.get_nearest_passable(x, y, -50.f, std::greater_equal<>());
                const float height_at_xy = karte.get_height_at(px, py);
                CHECK(height_at_xy >= -50.f);
            }
            {   // Höchstens
                const auto [px, py] = karte.get_nearest_passable(x, y, -50.f, std::less_equal<>());
                const float height_at_xy = karte.get_height_at(px, py);
                CHECK(height_at_xy <= -50.f);
            }
        }
    }
    SUBCASE("valide startpositionen") {
        for (unsigned i = 0; i < 100; ++i) {
            Welt welt(10);
            CHECK(welt.get_objektanzahl(Objekt::Typ::SUB_AI) >= 10);
            for (const auto& [key, o] : welt.get_objekte()) {
                if (o->get_typ() == Objekt::Typ::SUB_AI) {
                    const auto obj_hoehe = welt.get_karte().get_height_at(o->get_pos().x(), o->get_pos().y());
                    REQUIRE(obj_hoehe <= welt.START_DEPTH_SUB);
                }
            }
        }
    }
}

};
