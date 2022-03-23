#include <log.hpp>

#include "spielszene.hpp"

Spielszene::Spielszene(const std::string& ip) : klient(new Klient(ip)) {
    //
}

Spielszene::Spielszene(sf::Window* window)
    : window(window)
{

}

Spielszene::~Spielszene() {
    delete klient;
}

void Spielszene::key_pressed() {
    /*
    switch (key.sym) {
        case SDLK_RIGHT: camNode->yaw(Ogre::Degree(-1)); break;
        case SDLK_LEFT:  camNode->yaw(Ogre::Degree(1)); break;
        case SDLK_UP:    camNode->pitch(Ogre::Degree(1)); break;
        case SDLK_DOWN:  camNode->pitch(Ogre::Degree(-1)); break;
        case SDLK_m: {
            // Neues Spieler Sub geben (nur wenn keins vorhanden)
            if (player_sub) { Log::debug() << "New player_sub not needed\n"; break; }
            const std::string& antwort = klient->request(Net::AKTION_NEUES_UBOOT, 1);
            if (!antwort.empty()) {
                player_sub = Net::deserialize<Sub>(antwort);
                nav_ui = Nav_UI(klient);
                sonar_ui = Sonar_UI(&player_sub.value());
                waffen_ui = Waffen_UI(klient);
            }
            else Log::err() << "New player_sub not available\n";
        } break;

        // Tabs
        case SDLK_F1: tab = NAV;      break;
        case SDLK_F2: tab = SONAR;    break;
        case SDLK_F3: tab = WEAPONS;  break;
        case SDLK_F4: tab = THREE_D;  break;
        case SDLK_F5: tab = MAINMENU; break;
        default: break;
    }
     */
}

void Spielszene::sync() {
    // Net Sync
    if (static sf::Clock timer; timer.getElapsedTime().asMilliseconds() >= 500) {
        if (player_sub) {
            const std::string& antwort = klient->request(Net::REQUEST_SUB, player_sub->get_id());
            if (!antwort.empty()) player_sub = Net::deserialize<Sub>(antwort);
            else Log::err() << "Spielszene::" << __func__ << " no sub returned with ID " << player_sub->get_id() << '\n';
        }
        timer.restart();
    }
}

void Spielszene::show() {
    sync();

    /* TODO
    // Gfx Interpolieren (nur eigenes Sub)
    if (static sf::Clock timer_interpol; player_sub.has_value()) {
        player_sub->tick(nullptr, timer_interpol.getElapsedTime().asSeconds() / 1000.f);
        timer_interpol.restart();
    }
     */
    if (!player_sub) tab = MAINMENU; // Kein Sub? -> Hauptmenü

    // Welches Menü rendern?
    switch (tab) {
        case MAINMENU:  render_menu();      break;
        case NAV:       render_nav();       break;
        case SONAR:     render_sonar();     break;
        case WEAPONS:   render_weapons();   break;
        case THREE_D:   render_3d();        break;
        default:        tab = MAINMENU;     break;
    }
}
