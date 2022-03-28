#pragma once

#include <doctest.h>

#include "../src/sim/physik.hpp"

TEST_CASE("physik") {

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
            REQUIRE(v.x() == doctest::Approx(v_alt.x()));
            REQUIRE(v.y() == doctest::Approx(v_alt.y() + e));
            REQUIRE(v.z() == doctest::Approx(v_alt.z()));
        }
        // e nach rechts bewegen
        for (const auto e : entfernungen) for (auto v : vektoren) {
            auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.x());
            Physik::move(v, 90, e);
            REQUIRE(v.x() == doctest::Approx(v_alt.x() + e));
            REQUIRE(v.y() == doctest::Approx(v_alt.y()));
            REQUIRE(v.z() == doctest::Approx(v_alt.z()));
        }
        // e nach unten bewegen
        for (const auto e : entfernungen) for (auto v : vektoren) {
            const auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.y());
            Physik::move(v, 180, e);
            REQUIRE(v.x() == doctest::Approx(v_alt.x()));
            REQUIRE(v.y() == doctest::Approx(v_alt.y() - e));
            REQUIRE(v.z() == doctest::Approx(v_alt.z()));
        }
        // e nach links bewegen
        for (auto e : entfernungen) for (auto v : vektoren) {
            const auto v_alt = v;
            CAPTURE(e);
            CAPTURE(v_alt.x());
            Physik::move(v, 270, e);
            REQUIRE(v.x() == doctest::Approx(v_alt.x() - e));
            REQUIRE(v.y() == doctest::Approx(v_alt.y()));
            REQUIRE(v.z() == doctest::Approx(v_alt.z()));
        }
    }

    SUBCASE("winkel_diff") {
        REQUIRE(Physik::winkel_diff( 10,  20) == doctest::Approx(  10));
        REQUIRE(Physik::winkel_diff(  0, 190) == doctest::Approx(-170));
        REQUIRE(Physik::winkel_diff( 90, 200) == doctest::Approx( 110));
        REQUIRE(Physik::winkel_diff(-90,  10) == doctest::Approx( 100));
        REQUIRE(Physik::winkel_diff( 90,  90) == doctest::Approx(   0));
        REQUIRE(Physik::winkel_diff(270, 180) == doctest::Approx( -90));
    }

    SUBCASE("kurs") {
        REQUIRE(Physik::kurs(0, 0, 0,  5)  == doctest::Approx(  0));
        REQUIRE(Physik::kurs(0, 0, 5,  5)  == doctest::Approx( 45));
        REQUIRE(Physik::kurs(0, 0, 5,  0)  == doctest::Approx( 90));
        REQUIRE(Physik::kurs(0, 0, 5,  -5) == doctest::Approx(135));
        REQUIRE(Physik::kurs(0, 0, 0,  -5) == doctest::Approx(180));
        REQUIRE(Physik::kurs(0, 0, -5, -5) == doctest::Approx(225));
        REQUIRE(Physik::kurs(0, 0, -5,  0) == doctest::Approx(270));
        REQUIRE(Physik::kurs(0, 0, -5,  5) == doctest::Approx(315));
    }

    SUBCASE("winkel_tiefe") {
        REQUIRE(Physik::winkel_tiefe({10,20, 0}, {10,30,  10}) == doctest::Approx( 45));
        REQUIRE(Physik::winkel_tiefe({10,20, 0}, {10,30, -10}) == doctest::Approx(-45));
    }

    SUBCASE("distanz") {
        REQUIRE(Physik::distanz(  0,   0,  0,  0) == doctest::Approx( 0));
        REQUIRE(Physik::distanz(  2,   2,  5,  6) == doctest::Approx( 5));
        REQUIRE(Physik::distanz(-10,   0, 10,  0) == doctest::Approx(20));
        REQUIRE(Physik::distanz(  0, -10,  0, 10) == doctest::Approx(20));
    }

    SUBCASE("distanz_xy") {
        REQUIRE(Physik::distanz_xy({2,2,2}, {5,6,9}) == doctest::Approx( 5));
    }

    SUBCASE("distanz_xyz") {
        REQUIRE(Physik::distanz_xyz({2,2,2}, {5,6,2}) == doctest::Approx( 5));
    }

    SUBCASE("bremsweg") {
        REQUIRE(Physik::bremsweg( 10,   1) == doctest::Approx(50));
        REQUIRE(Physik::bremsweg( 10,  -1) == doctest::Approx(50));
        REQUIRE(Physik::bremsweg(-10,   1) == doctest::Approx(50));
        REQUIRE(Physik::bremsweg(-10,  -1) == doctest::Approx(50));
    }

}
