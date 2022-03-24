#pragma once

#ifdef TESTS_AKTIV
#define DOCTEST_CONFIG_IMPLEMENT
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include <doctest.h>

#include "test_physik.hpp"
#include "test_objekte.hpp"
#include "test_welt.hpp"
#endif // TESTS_AKTIV

namespace test {

    int run(int argc, char** argv) {
        #ifdef TESTS_AKTIV
        doctest::Context context;
        context.setOption("order-by", "name");
        context.applyCommandLine(argc, argv);
        const auto res = context.run();
        context.shouldExit();
        return res;
        #else
        std::cout << "Tests deaktiviert - Tests muessen via Makro TESTS_AKTIV aktiviert werden";
        return 1;
        #endif
    }

}
