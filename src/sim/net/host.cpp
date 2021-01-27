#include "host.hpp"
#include "net.hpp"

#include <log.hpp>

Host::Host(uint16_t port) {
    ENetAddress address {
        .host = ENET_HOST_ANY,
        .port = Net::PORT
    };
    if (server = enet_host_create(&address, 32, 2, 0, 0); server == nullptr) {
        Log::err() << "An error occurred while trying to create an ENet server host.\n";
        exit(EXIT_FAILURE);
    }

}

void Host::start() {
    while (loop) {
        welt.tick();

        ENetEvent event;
        while(enet_host_service(server, &event, 1000) > 0) switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                       event.peer->address.host,
                       event.peer->address.port);
                       //event.peer->data = "Client information";
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                       event.packet->dataLength,
                       event.packet->data,
                       event.peer->data,
                       event.channelID);
                enet_packet_destroy(event.packet);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                printf("%s disconnected.\n", event.peer->data);
                event.peer->data = nullptr; // delete wenn nötig
                break;
        }
    }
}

void Host::stop() {
    loop = false;
}

Host::~Host() {
    enet_host_destroy(server);
}
