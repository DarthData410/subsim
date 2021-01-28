#include "gfx/ui.hpp"
#include "fenster/szene.hpp"
#include "sim/net/host.hpp"
#include "sim/net/net.hpp"

#include <OgreRoot.h>
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
        Szene app;
        app.initApp();
        app.getRoot()->startRendering();
        app.closeApp();
    } catch (const std::exception& e) { std::cerr << e.what() << std::endl; }

    #if defined(__linux__)
        if (server) std::this_thread::sleep_for(std::chrono::seconds(1)); // X11 workaround
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

/*
if (body) {
    const auto& pos = body->getTransform().getPosition();
    const auto& rot = body->getTransform().getOrientation();
    const auto& v_l = body->getLinearVelocity();
    const auto& v_a = body->getAngularVelocity();

    ImGui::Text("Pos: %s", pos.to_string().c_str());
    ImGui::Text("Rot: %s", rot.to_string().c_str());
    ImGui::Text("V/l:   %s", v_l.to_string().c_str());
    ImGui::Text("V/a:   %s", v_a.to_string().c_str());
}
 */

/*
case SDLK_UP:
welt.body->applyForceToCenterOfMass({100'000'000,0,0});
case SDLK_DOWN:
//welt.body->applyForceToCenterOfMass({-100'000'000,0,0});
break;
case SDLK_LEFT:
//welt.body->applyTorque({0, -1'000'000, 0});
break;
case SDLK_RIGHT: {
rp3d::Vector3 v = welt.body->getLinearVelocity();
Physik::rotate2d(v, 0.01);
welt.body->setLinearVelocity(v);
} break;
 */
