#include <iostream>

#if defined(UNITTEST)
    #include "../test/test.hpp"
#else
    #include "fenster/szene.hpp"
    #include "sim/net/host.hpp"
    #include "sim/net/net.hpp"
    #include <nada/log.hpp>
    #include <thread>

void start_server(Host* host) {
    nada::Log::out() << "Server startet..." << nada::Log::flush;
    host->start();
}
#endif

int main(int argc, char** argv) {
#if defined(UNITTEST)
    std::cout << "starte tests\n";
    return test::run(argc, argv);
}
#else
    for (int i = 0; i < argc; i++) {
        const std::string arg(argv[i]);
    }
    // Enet init
    if (enet_initialize() != 0) {
        std::cerr << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    } else atexit(enet_deinitialize);

    std::unique_ptr<Host> host(new Host(Net::PORT));
    std::unique_ptr<std::thread> host_thread(new std::thread(start_server, host.get()));

    try {
        Szene szene;
        szene.show();
    } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }

    #if defined(__linux__)
        if (host) std::this_thread::sleep_for(std::chrono::seconds(1)); // X11 workaround
    #endif

    // Host stoppen
    if (host && host_thread) {
        nada::Log::out() << "Stopping server... " << nada::Log::endl;
        host->stop();
        host_thread->join();
        nada::Log::out() << "Server stopped." << nada::Log::endl;
    }
}
#endif
