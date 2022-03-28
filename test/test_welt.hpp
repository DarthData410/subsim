#pragma once

#include <doctest.h>

TEST_CASE("welt") {
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
        welt.add_new_sub(1, true);
        welt.add_new_sub(1, true);
        REQUIRE(welt.get_objekte().size() == 2);
        bool zone_eingenommen = false;
        for (unsigned i = 0; i < 100'000; ++i) {
            welt.tick(10);
            for (const auto& zone : welt.get_zonen()) if (zone.get_team() == 1) { zone_eingenommen = true; break; }
        }
        REQUIRE(zone_eingenommen == true);
    }
}
