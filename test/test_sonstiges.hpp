#pragma once

#include "../src/sim/game/objekte/sub_ai.hpp"
#include "../src/sim/net/net.hpp"
#include <doctest.h>
#include <memory>

TEST_CASE("serialisierung") {

    SUBCASE("explosion") {
        // TODO
    }

    SUBCASE("OBJEKT_STEUERBAR") {
        const std::unique_ptr<Objekt> o(new Objekt_Steuerbar({0,0,0}, Motor(), Motor(), Motor()));
        CHECK(o->get_typ() == Objekt::Typ::OBJEKT_STEUERBAR);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::OBJEKT_STEUERBAR);
        CHECK(dynamic_cast<Objekt_Steuerbar*>(o_serialisiert.get()));
    }

    SUBCASE("SUB") {
        const std::unique_ptr<Objekt> o(new Sub({0,0,0}, Motor(), Motor(), Motor()));
        CHECK(o->get_typ() == Objekt::Typ::SUB);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::SUB);
        CHECK(dynamic_cast<Sub*>(o_serialisiert.get()));
    }

    SUBCASE("SUB_AI") {
        const std::unique_ptr<Objekt> o(new Sub_AI(Sub({0,0,0}, Motor(), Motor(), Motor())));
        CHECK(o->get_typ() == Objekt::Typ::SUB_AI);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::SUB_AI);
        CHECK(dynamic_cast<Sub_AI*>(o_serialisiert.get()));
    }

    SUBCASE("TORPEDO") {
        const std::unique_ptr<Objekt> o(new Torpedo(Motor(), Motor(), Motor(), "test_torp", 100));
        CHECK(o->get_typ() == Objekt::Typ::TORPEDO);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::TORPEDO);
        CHECK(dynamic_cast<Torpedo*>(o_serialisiert.get()));
    }

}
