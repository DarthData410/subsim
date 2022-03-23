#include "spielszene.hpp"

#include <log.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Window/Event.hpp>

Spielszene::Spielszene(sf::RenderWindow* window, const std::string& ip) :
    klient(new Klient(ip)), window(window)
{
    assert(window);
}

void Spielszene::key_pressed(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Escape: window->close(); break;

        // Neues Spieler Sub geben (nur wenn keins vorhanden)
        case sf::Keyboard::M: {
            if (player_sub) { Log::debug() << "New player_sub not needed\n"; break; }
            const std::string& antwort = klient->request(Net::AKTION_NEUES_UBOOT, 1);
            if (!antwort.empty()) {
                player_sub = Net::deserialize<Sub>(antwort);
                nav_ui     = Nav_UI(klient.get());
                sonar_ui   = Sonar_UI(&player_sub.value());
                waffen_ui  = Waffen_UI(klient.get());
            }
            else Log::err() << "New player_sub not available\n";
        } break;

        // Tabs
        case sf::Keyboard::F1: tab = NAV;      break;
        case sf::Keyboard::F2: tab = SONAR;    break;
        case sf::Keyboard::F3: tab = WEAPONS;  break;
        case sf::Keyboard::F4: tab = THREE_D;  break;
        case sf::Keyboard::F5: tab = MAINMENU; break;
        default: break;
    }
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

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
                case sf::Event::KeyReleased: key_pressed(event.key.code); break;
                case sf::Event::Closed: window->close(); break;
            }
        }


        /* TODO
        // Gfx Interpolieren (nur eigenes Sub)
        if (static sf::Clock timer_interpol; player_sub.has_value()) {
            player_sub->tick(nullptr, timer_interpol.getElapsedTime().asSeconds() / 1000.f);
            timer_interpol.restart();
        }
         */
        if (!player_sub) tab = MAINMENU; // Kein Sub? -> Hauptmenü

        static sf::Clock fps_clock;
        ImGui::SFML::Update(*window, fps_clock.restart());

        ImGui::Begin("Spielszene");

        // Welches Menü rendern?
        switch (tab) {
            case MAINMENU:  render_menu();      break;
            case NAV:       render_nav();       break;
            case SONAR:     render_sonar();     break;
            case WEAPONS:   render_weapons();   break;
            case THREE_D:   render_3d();        break;
            default:        tab = MAINMENU;     break;
        }

        ImGui::End();

        // SFML Draws
        window->clear();
        ImGui::SFML::Render(*window);
        window->display();
    }
}
