#pragma once

#include <doctest.h>
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

};
