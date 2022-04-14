#pragma once

#include "../src/sim/game/objekte/sub_ai.hpp"
#include "../src/sim/net/net.hpp"
#include <doctest.h>
#include <memory>

TEST_CASE("serialisierung") {

    SUBCASE("OBJEKT_STEUERBAR") {
        const std::unique_ptr<Objekt> o(new Objekt_Steuerbar({0,0,0}, Motor(), Motor(), Motor()));
        CHECK(o->get_typ() == Objekt::Typ::OBJEKT_STEUERBAR);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::OBJEKT_STEUERBAR);
        const auto o_cast = dynamic_cast<Objekt_Steuerbar*>(o_serialisiert.get());
        CHECK(o_cast != nullptr); // Ist korrekt polymorph serialisiert worden?
        const auto o_kopie = std::unique_ptr<Objekt>(new Objekt_Steuerbar(*o_cast));
        CHECK(s == Net::serialize(o_kopie)); // Ist eine serialisierte Kopie identisch mit dem serialisierten Original?
    }

    SUBCASE("SUB") {
        const std::unique_ptr<Objekt> o(new Sub("Test Sub", {0,0,0}, Motor(), Motor(), Motor()));
        CHECK(o->get_typ() == Objekt::Typ::SUB);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::SUB);
        const auto o_cast = dynamic_cast<Sub*>(o_serialisiert.get());
        CHECK(o_cast != nullptr);
        const auto o_kopie = std::unique_ptr<Objekt>(new Sub(*o_cast));
        CHECK(s == Net::serialize(o_kopie));
    }

    SUBCASE("SUB_AI") {
        const std::unique_ptr<Objekt> o(new Sub_AI(Sub("Test AI-Sub", {0,0,0}, Motor(), Motor(), Motor())));
        CHECK(o->get_typ() == Objekt::Typ::SUB_AI);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::SUB_AI);
        const auto o_cast = dynamic_cast<Sub_AI*>(o_serialisiert.get());
        CHECK(o_cast != nullptr);
        const auto o_kopie = std::unique_ptr<Objekt>(new Sub_AI(*o_cast));
        CHECK(s == Net::serialize(o_kopie));
    }

    SUBCASE("TORPEDO") {
        const std::unique_ptr<Objekt> o(new Torpedo(Motor(), Motor(), Motor(), "test_torp", 100, Explosion()));
        CHECK(o->get_typ() == Objekt::Typ::TORPEDO);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::TORPEDO);
        const auto o_cast = dynamic_cast<Torpedo*>(o_serialisiert.get());
        CHECK(o_cast != nullptr);
        const auto o_kopie = std::unique_ptr<Objekt>(new Torpedo(*o_cast));
        CHECK(s == Net::serialize(o_kopie));
    }

    SUBCASE("explosion") {
        const std::unique_ptr<Objekt> o(new Explosion(10, 20, 30));
        CHECK(o->get_typ() == Objekt::Typ::EXPLOSION);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::EXPLOSION);
        const auto o_cast = dynamic_cast<Explosion*>(o_serialisiert.get());
        CHECK(o_cast != nullptr);
        const auto o_kopie = std::unique_ptr<Objekt>(new Explosion(*o_cast));
        CHECK(s == Net::serialize(o_kopie));
    }

    SUBCASE("ping") {
        const std::unique_ptr<Objekt> sub(new Sub("Test Sub", {0,0,0}, Motor(), Motor(), Motor()));
        const std::unique_ptr<Objekt> o(new Ping(sub.get(), 20, 30));
        CHECK(o->get_typ() == Objekt::Typ::PING);
        const auto& s = Net::serialize(o);
        const auto& o_serialisiert = Net::deserialize<std::unique_ptr<Objekt>>(s);
        CHECK(o_serialisiert->get_typ() == Objekt::Typ::PING);
        const auto o_cast = dynamic_cast<Ping*>(o_serialisiert.get());
        CHECK(o_cast != nullptr);
        const auto o_kopie = std::unique_ptr<Objekt>(new Ping(*o_cast));
        CHECK(s == Net::serialize(o_kopie));
    }

}
