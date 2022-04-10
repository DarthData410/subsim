#pragma once

#define DOCTEST_CONFIG_IMPLEMENT
//#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include <doctest.h>

#include "test_physik.hpp"
#include "test_objekte.hpp"
#include "test_welt.hpp"
#include "test_sonstiges.hpp"

namespace test {

    int run(int argc, char** argv) {
        doctest::Context context;
        context.setOption("order-by", "name");
        context.applyCommandLine(argc, argv);
        const auto res = context.run();
        context.shouldExit();
        return res;
    }

}
