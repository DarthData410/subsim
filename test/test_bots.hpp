#pragma once

#include <doctest.h>
#include "../src/sim/welt.hpp"
#include "../src/sim/game/objekte/sub_ai.hpp"

class Test_Bots {

TEST_CASE_CLASS("bots") {

    Welt welt(0);
    SUBCASE("bot vs bot") {
        // 2 Bots initialisieren
        Sub_AI* sub1;
        Sub_AI* sub2;
        REQUIRE_NOTHROW(sub1 = dynamic_cast<Sub_AI*>(welt.objekte.at(welt.add_new_sub(1, true)->get_id()).get()));
        REQUIRE_NOTHROW(sub2 = dynamic_cast<Sub_AI*>(welt.objekte.at(welt.add_new_sub(2, true)->get_id()).get()));
        REQUIRE(sub1 != nullptr);
        REQUIRE(sub2 != nullptr);
        REQUIRE_EQ(welt.get_objektanzahl(Objekt::Typ::SUB_AI), 2);

        // Abstand verringern
        const int abstand = 5000;
        sub1->pos = {sub2->pos.x() - abstand, sub2->pos.y(), sub2->pos.z()};
        for (unsigned i = 0; i < 1500; ++i) welt.tick(1.f);
        CHECK_LT(welt.get_objektanzahl(Objekt::Typ::SUB_AI), 2);
    }

}

};
