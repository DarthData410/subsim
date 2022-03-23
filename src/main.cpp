#include "fenster/szene.hpp"
#include "sim/net/host.hpp"
#include "sim/net/net.hpp"

#include <iostream>
#include <log.hpp>

void start_server(Host*& host) {
    Log::out() << "Server startet..." << Log::flush;
    host = new Host(Net::PORT);
    host->start();
}

int main(int, char**) {
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
        Log::out() << "Server stop.\n";
        host->stop();
        host_thread->join();
        delete host;
        delete host_thread;
    }
    return 0;
}
