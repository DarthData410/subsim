#pragma once

#include <doctest.h>
#include "../src/sim/welt.hpp"
#include "../src/sim/game/objekte/ping.hpp"

class Test_Objekte {

    TEST_CASE_CLASS("Ping") {
        Ping ping;
        const dist_t range = 100;
        ping.range = range;
        const auto noise = ping.get_noise();
        CHECK(ping.get_noise_relative(0) == noise);
        CHECK(ping.get_noise_relative(range * 0.5)  == doctest::Approx(noise * 0.5));
        CHECK(ping.get_noise_relative(range * 0.25) == doctest::Approx(noise * 0.75));
        CHECK(ping.get_noise_relative(range * 0.75) == doctest::Approx(noise * 0.25));
    }

    TEST_CASE_CLASS("Torpedo Zielauswahl via AS detektionen") {
        Welt welt(0);
        const auto sub_id = welt.add_new_sub(1, false)->get_id();
        Sub* sub = nullptr;
        CHECK_NOTHROW(sub = dynamic_cast<Sub*>(welt.get_objekte().at(sub_id).get()));
        CHECK(sub != nullptr);
        REQUIRE(sub->get_torpedos().empty() == false);
        const auto& torpedotyp = sub->get_torpedos().begin()->first;
        Torpedo t(torpedotyp);
        t.kurs = 90.f; // <------ wichtig für die Zielauswahl unten
        REQUIRE(t.sonar_aktiv.has_value());
        auto& as = t.sonar_aktiv.value();
        SUBCASE("Auswahl durch passenden Kurs") {
            as.detektionen.emplace_back(Detektion(
                    1, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.8f, 90.f, 500.0, -50.0 // kurs wie Torpedo
            ));
            as.detektionen.emplace_back(Detektion(
                    2, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.8f, 10.f, 500.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    3, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.8f, 270.f, 500.0, -50.0
            ));
            const Detektion* d1 = t.get_beste_detektion();
            CHECK(d1->objekt_id == 1);
        }
        SUBCASE("Auswahl durch Detektionstyp") {
            as.detektionen.emplace_back(Detektion(
                    1, Detektion::Typ::ACTIVE_SONAR_ECHO, // zu bevorzugener Typ
                    0.8f, 90.f, 500.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    2, Detektion::Typ::MOVEMENT_SIGNATURE,
                    0.8f, 90.f, 500.0, -50.0
            ));
            const Detektion* d1 = t.get_beste_detektion();
            CHECK(d1->objekt_id == 1);
        }
        SUBCASE("Auswahl durch Entfernung") {
            as.detektionen.emplace_back(Detektion(
                    1, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.8f, 90.f, 500.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    2, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.8f, 90.f, 400.0, -50.0 // zu bevorzugende Entfernung
            ));
            const Detektion* d1 = t.get_beste_detektion();
            CHECK(d1->objekt_id == 2);
        }
        SUBCASE("Auswahl durch Richtung + Noise 1") {
            as.detektionen.emplace_back(Detektion(
                    1, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.3f, 60.f, 400.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    2, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.4f, 70.f, 400.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    3, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.5f, 80.f, 400.0, -50.0
            ));
            const Detektion* d1 = t.get_beste_detektion();
            CHECK(d1->objekt_id == 3);
        }
        SUBCASE("Auswahl durch Richtung + Noise 2") {
            as.detektionen.emplace_back(Detektion(
                    1, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.9f, 60.f, 400.0, -50.0 // wegen hohem noise attraktiv
            ));
            as.detektionen.emplace_back(Detektion(
                    2, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.1f, 70.f, 400.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    3, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.1f, 80.f, 400.0, -50.0
            ));
            const Detektion* d1 = t.get_beste_detektion();
            CHECK(d1->objekt_id == 1);
        }
        SUBCASE("Auswahl durch Richtung + Noise 3") {
            as.detektionen.emplace_back(Detektion(
                    1, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.9f, 250.f, 400.0, -50.0 // sollte trotz hohem noise zu hoher Kursdiff sein
            ));
            as.detektionen.emplace_back(Detektion(
                    2, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.2f, 70.f, 400.0, -50.0
            ));
            as.detektionen.emplace_back(Detektion(
                    3, Detektion::Typ::ACTIVE_SONAR_ECHO,
                    0.2f, 80.f, 400.0, -50.0
            ));
            const Detektion* d1 = t.get_beste_detektion();
            CHECK(d1->objekt_id == 3);
        }
    }

};
