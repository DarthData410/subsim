#include "klient.hpp"
#include "net.hpp"

#include <log.hpp>
#include <thread>

Klient::Klient(const std::string& ip) : host_ip(ip) {
    klient = enet_host_create(NULL, // 0 = Klient
                              1,  // Verbindungen
                              2,  // Kanäle
                              0,0 // Bandbreite
    );
    if (klient == NULL) {
        Log::err() << "Klient: An error occurred while trying to create an ENet client host.\n";
        return;
    }
    Log::debug() << "Klient: ()\n";
    connect();
    test();
    flush_thread = std::thread(&Klient::keep_alive, this);
}

Klient::~Klient() {
    alive = false;
    flush_thread.join();
    if (klient) enet_host_destroy(klient);
}

bool Klient::connect() {
    ENetAddress host_address;
    enet_address_set_host(&host_address, host_ip.c_str());
    host_address.port = Net::PORT;
    if (server = enet_host_connect(klient, &host_address, 2, 0); server == NULL) {
        Log::err() << "Klient: No available peers for initiating an ENet connection.\n";
        return false;
    }
    ENetEvent event;
    if (enet_host_service(klient, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
        Log::out() << "Klient: Connection established to Host " << host_ip << Log::endl;
        return true;
    }
    enet_peer_reset(server);
    Log::err() << "Klient: Connection failed to Host " << host_ip << Log::endl;
    return false;
}

void Klient::test() {
    if (server) {
        std::cout << "Klient: wuppi." << std::endl;
        ENetPacket* packet = enet_packet_create ("packet",
                                                 strlen ("packet") + 1,
                                                 ENET_PACKET_FLAG_RELIABLE);
        //enet_packet_resize (packet, strlen ("packetfoo") + 1);
        //strcpy (& packet -> data [strlen ("packet")], "foo");
        enet_peer_send(server, 0, packet);
    }
}

void Klient::keep_alive() {
    while (alive.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ENetEvent event;
        std::scoped_lock lock(connection_mutex);
        enet_host_service(klient, &event, 5000);
    }
}
