#include "host.hpp"
#include "net.hpp"
#include "kommando.hpp"

#include <log.hpp>

Host::Host(uint16_t port) {
    ENetAddress address {
        .host = ENET_HOST_ANY,
        .port = port
    };
    if (server = enet_host_create(&address, 32, 2, 0, 0); !server) {
        Log::err() << "An error occurred while trying to create an ENet server host.\n";
        exit(EXIT_FAILURE);
    }
}

void Host::start() {
    while (loop) {
        welt.tick();

        Log::out() << "Server eventloop" << Log::endl;
        ENetEvent event;
        while(enet_host_service(server, &event, 1000) > 0) switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                printf("A new client connected from %x:%u.\n",
                       event.peer->address.host,
                       event.peer->address.port);
                       //event.peer->data = "Client information";
                break;

            case ENET_EVENT_TYPE_RECEIVE: {
                printf("Server: A packet of length [%u] containing [%s] was received from [%s] on channel [%u].\n",
                       event.packet->dataLength, event.packet->data, event.peer->data, event.channelID);
                std::string data((const char*) event.packet->data);
                std::cout << "Data = " << data << std::endl;

                // Anfrage parsen
                std::stringstream ss;
                Net::Deserializer ds(ss);
                Net::Request request;
                ds >> request;
                if (request == Net::SUB_CMD) {
                    Kommando kommando;
                    ds >> kommando;
                    kommando.apply(&welt);
                } else Log::err() << "\tUnknown Request Net::" << request << '\n';
                enet_packet_destroy(event.packet); // Aufräumen
                } break;

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
    stop();
    enet_host_destroy(server);
}
