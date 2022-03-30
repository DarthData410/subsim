#include <iostream>

#if defined(UNITTEST)
    #include "../test/test.hpp"
#else
    #include "fenster/szene.hpp"
    #include "sim/net/host.hpp"
    #include "sim/net/net.hpp"
    #include <log.hpp>

void start_server(Host*& host) {
    Log::out() << "Server startet..." << Log::flush;
    host = new Host(Net::PORT);
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

    Host* host = nullptr;
    std::thread* host_thread = nullptr;
    host_thread = new std::thread(start_server, std::ref(host));

    try {
        Szene szene;
        szene.show();
    } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }

    #if defined(__linux__)
        if (host) std::this_thread::sleep_for(std::chrono::seconds(1)); // X11 workaround
    #endif

    // Host stoppen
    if (host && host_thread) {
        Log::out() << "Stopping server... " << Log::endl;
        host->stop();
        host_thread->join();
        delete host;
        delete host_thread;
        Log::out() << "Server stopped." << Log::endl;
    }
    return 0;
}
#endif
