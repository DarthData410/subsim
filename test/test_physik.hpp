#pragma once

#include <doctest.h>

#include "../src/sim/physik.hpp"
#include "../src/sim/game/objekte/sub.hpp"

class Test_Physik {
TEST_CASE_CLASS("physik") {

    SUBCASE("move und get_punkt") {
        const std::vector<Vektor> vektoren {
                {  0,   0,   0},
                { 10,  20,  30},
                {-10, -20, -30},
        };
        const std::vector<double> entfernungen { -10, 0, 10 };

        // e nach oben bewegen
        for (auto e : entfernungen) for (auto v : vektoren) {
            const auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.y());
            Physik::move(v, 0, e);
            CHECK(v.x() == doctest::Approx(v_alt.x()));
            CHECK(v.y() == doctest::Approx(v_alt.y() + e));
            CHECK(v.z() == doctest::Approx(v_alt.z()));
        }
        // e nach rechts bewegen
        for (const auto e : entfernungen) for (auto v : vektoren) {
            auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.x());
            Physik::move(v, 90, e);
            CHECK(v.x() == doctest::Approx(v_alt.x() + e));
            CHECK(v.y() == doctest::Approx(v_alt.y()));
            CHECK(v.z() == doctest::Approx(v_alt.z()));
        }
        // e nach unten bewegen
        for (const auto e : entfernungen) for (auto v : vektoren) {
            const auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.y());
            Physik::move(v, 180, e);
            CHECK(v.x() == doctest::Approx(v_alt.x()));
            CHECK(v.y() == doctest::Approx(v_alt.y() - e));
            CHECK(v.z() == doctest::Approx(v_alt.z()));
        }
        // e nach links bewegen
        for (auto e : entfernungen) for (auto v : vektoren) {
            const auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.x());
            Physik::move(v, 270, e);
            CHECK(v.x() == doctest::Approx(v_alt.x() - e));
            CHECK(v.y() == doctest::Approx(v_alt.y()));
            CHECK(v.z() == doctest::Approx(v_alt.z()));
        }
    }

    SUBCASE("winkel_diff") {
        CHECK(Physik::winkel_diff( 10,  20) == doctest::Approx(  10));
        CHECK(Physik::winkel_diff(  0, 190) == doctest::Approx(-170));
        CHECK(Physik::winkel_diff( 90, 200) == doctest::Approx( 110));
        CHECK(Physik::winkel_diff(-90,  10) == doctest::Approx( 100));
        CHECK(Physik::winkel_diff( 90,  90) == doctest::Approx(   0));
        CHECK(Physik::winkel_diff(270, 180) == doctest::Approx( -90));
    }

    SUBCASE("kurs") {
        CHECK(Physik::kurs(0, 0, 0,  5)  == doctest::Approx(  0));
        CHECK(Physik::kurs(0, 0, 5,  5)  == doctest::Approx( 45));
        CHECK(Physik::kurs(0, 0, 5,  0)  == doctest::Approx( 90));
        CHECK(Physik::kurs(0, 0, 5,  -5) == doctest::Approx(135));
        CHECK(Physik::kurs(0, 0, 0,  -5) == doctest::Approx(180));
        CHECK(Physik::kurs(0, 0, -5, -5) == doctest::Approx(225));
        CHECK(Physik::kurs(0, 0, -5,  0) == doctest::Approx(270));
        CHECK(Physik::kurs(0, 0, -5,  5) == doctest::Approx(315));
        CHECK(Physik::kurs({0, 0, 87},  {5,  5, 29})  == doctest::Approx( 45));
        CHECK(Physik::kurs({0, 0, 87},  {0,  5, 29})  == doctest::Approx(  0));
        CHECK(Physik::kurs({0, 0, 87},  {5,  0, 29})  == doctest::Approx( 90));
        CHECK(Physik::kurs({0, 0, 87},  {5,  -5, 29}) == doctest::Approx(135));
        CHECK(Physik::kurs({0, 0, 87},  {0,  -5, 29}) == doctest::Approx(180));
        CHECK(Physik::kurs({0, 0, 87},  {-5, -5, 29}) == doctest::Approx(225));
        CHECK(Physik::kurs({0, 0, 87},  {-5,  0, 29}) == doctest::Approx(270));
        CHECK(Physik::kurs({0, 0, 87},  {-5,  5, 29}) == doctest::Approx(315));
    }

    SUBCASE("kurs relativ") {
        Sub s1, s2;
        s1.pos  = {0,0,-50};
        s2.pos  = {100, 100, -50};
        s2.kurs = 0; // egal

        s1.kurs = 0;
        CHECK(Physik::kurs_relativ(&s1, &s2) == doctest::Approx(45));
        s1.kurs = 40;
        CHECK(Physik::kurs_relativ(&s1, &s2) == doctest::Approx(5));
        s1.kurs = 45;
        CHECK(Physik::kurs_relativ(&s1, &s2) == doctest::Approx(0));
        s1.kurs = 90;
        CHECK(Physik::kurs_relativ(&s1, &s2) == doctest::Approx(-45));
    }

    SUBCASE("winkel_tiefe") {
        CHECK(Physik::winkel_tiefe({10,20, 0}, {10,30,  10}) == doctest::Approx( 45));
        CHECK(Physik::winkel_tiefe({10,20, 0}, {10,30, -10}) == doctest::Approx(-45));
    }

    SUBCASE("winkel_zwischen") {
        CHECK(Physik::is_winkel_im_bereich(0, 0, 10) == true);
        CHECK(Physik::is_winkel_im_bereich(-10, 0, 15) == true);
        CHECK(Physik::is_winkel_im_bereich(10, 0, 15) == true);
        CHECK(Physik::is_winkel_im_bereich(359, 0, 10) == true);
        CHECK(Physik::is_winkel_im_bereich(360, 0, 10) == true);
        CHECK(Physik::is_winkel_im_bereich(361, 0, 10) == true);
        CHECK(Physik::is_winkel_im_bereich(90, 90, 45) == true);
        CHECK(Physik::is_winkel_im_bereich(90, -90, 45) == false);
        CHECK(Physik::is_winkel_im_bereich(270, -90, 45) == true);
        CHECK(Physik::is_winkel_im_bereich(170, 180, 45) == true);
        CHECK(Physik::is_winkel_im_bereich(190, 180, 45) == true);
        CHECK(Physik::is_winkel_im_bereich(15, 180, 160) == false);
        CHECK(Physik::is_winkel_im_bereich(-15, 180, 160) == false);
        CHECK(Physik::is_winkel_im_bereich(90, 180, 120) == true);
        CHECK(Physik::is_winkel_im_bereich(-90, 180, 120) == true);
    }

    SUBCASE("distanz") {
        CHECK(Physik::distanz(  0,   0,  0,  0) == doctest::Approx( 0));
        CHECK(Physik::distanz(  2,   2,  5,  6) == doctest::Approx( 5));
        CHECK(Physik::distanz(-10,   0, 10,  0) == doctest::Approx(20));
        CHECK(Physik::distanz(  0, -10,  0, 10) == doctest::Approx(20));
    }

    SUBCASE("distanz_xy") {
        CHECK(Physik::distanz_xy({2,2,2}, {5,6,9}) == doctest::Approx( 5));
    }

    SUBCASE("distanz_xyz") {
        CHECK(Physik::distanz_xyz({2,2,2}, {5,6,2}) == doctest::Approx( 5));
    }

    SUBCASE("in_reichweite_xyz") {
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {100, 0,  0}, 100) == true);
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {0, 100,  0}, 100) == true);
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {100, 1,  0}, 100) == false);
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {1, 100,  0}, 100) == false);
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {50, 50,  0}, 100) == true);
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {75, 75,  0}, 100) == false);
        CHECK(Physik::in_reichweite_xyz({0, 0, 0}, {75, 75, 75}, 100) == false);
    }

    SUBCASE("bremsweg") {
        CHECK(Physik::bremsweg( 10,   1) == doctest::Approx(50));
        CHECK(Physik::bremsweg( 10,  -1) == doctest::Approx(50));
        CHECK(Physik::bremsweg(-10,   1) == doctest::Approx(50));
        CHECK(Physik::bremsweg(-10,  -1) == doctest::Approx(50));
    }

    SUBCASE("schallfaktor") {
        CHECK(Physik::schallfaktor(10000)  == doctest::Approx(0.833333));
        CHECK(Physik::schallfaktor(50000)  == doctest::Approx(0.16667));
        CHECK(Physik::schallfaktor(100000) == doctest::Approx(0.04762));
    }

    SUBCASE("sichtbarkeit") {
        CHECK(Physik::sichtbarkeit(1.0,  7.5,    500) == doctest::Approx(0.90486));
        CHECK(Physik::sichtbarkeit(0.75, 7.5,  10000) == doctest::Approx(0.68046));
        CHECK(Physik::sichtbarkeit(0.5,  7.5,    500) == doctest::Approx(0.63422));
        CHECK(Physik::sichtbarkeit(0.5,    0, 100000) == doctest::Approx(0.00626));
        CHECK(Physik::sichtbarkeit(0.25,  10,   5000) == doctest::Approx(0.56368));
        CHECK(Physik::sichtbarkeit(0.25, -10,   5000) == doctest::Approx(0.56368));
    }

}
};