#include "spielszene.hpp"
#include "gfx/ui.hpp"

#include <log.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

#include "../sim/game/objekte/sub.hpp"
#include "../sim/net/klient.hpp"
#include "nav_ui.hpp"
#include "sonar_ui.hpp"
#include "waffen_ui.hpp"
#include "map_ui.hpp"

Spielszene::Spielszene(sf::RenderWindow* window, const std::string& ip) :
    klient(new Klient(ip)), window(window)
{
    assert(window);
}

void Spielszene::sync() {
    // Net Sync
    if (static sf::Clock timer; timer.getElapsedTime().asMilliseconds() >= 500) {
        if (player_sub) {
            const std::string& antwort = klient->request(Net::REQUEST_SUB, player_sub->get_id());
            if (!antwort.empty()) {
                player_sub = std::make_unique<Sub>(Net::deserialize<Sub>(antwort));
                Log::debug() << "sync sub id=" << player_sub->get_id() << '\n';
            }
            else Log::err() << "Spielszene::" << __func__ << " no sub returned with ID " << player_sub->get_id() << '\n';
        }
        timer.restart();
    }
}

void Spielszene::key_pressed(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Escape: window->close(); break;
        case sf::Keyboard::M: { // Neues Spiel: Sub geben (nur wenn keins vorhanden) && UIs init
            if (player_sub) { Log::debug() << "New player_sub not needed\n"; break; }
            const std::string& antwort = klient->request(Net::AKTION_NEUES_UBOOT, 1); // objekt_id = Team
            if (!antwort.empty()) {
                player_sub = std::make_unique<Sub>(Net::deserialize<Sub>(antwort));
                map_ui     = std::make_unique<Map_UI>(klient.get());
                nav_ui     = std::make_unique<Nav_UI>(klient.get());
                sonar_ui   = std::make_unique<Sonar_UI>(klient.get());
                waffen_ui  = std::make_unique<Waffen_UI>(klient.get());
                Log::debug() << "New player_sub id=" << player_sub->get_id() << '\n';
            }
            else Log::err() << "New player_sub not available\n";
        } break;
        // Change Tab
        case sf::Keyboard::F1: tab = MAP;      break;
        case sf::Keyboard::F2: tab = NAV;      break;
        case sf::Keyboard::F3: tab = SONAR;    break;
        case sf::Keyboard::F4: tab = WEAPONS;  break;
        case sf::Keyboard::F5: tab = THREE_D;  break;
        case sf::Keyboard::F6: tab = MAINMENU; break;
        default: break;
    }
}

void Spielszene::draw_menu() {
    // Timelapse
    ImGui::Begin("Mainmenu");
    ui::Text("Server Timelapse = %.1f", klient->get_timelapse());
    static float timelapse = 1;
    ui::SliderFloat("Timelapse", &timelapse, 0, 20, "%.1f");
    if (ImGui::SameLine(); ui::Button("Set")) {
        Log::debug() << "Timelapse should now be: " << timelapse << '\n';
        const Kommando neue_zeit_kommando(Kommando::TIMELAPSE, 0, timelapse);
        klient->kommando(neue_zeit_kommando);
    }
    ImGui::End();
}

void Spielszene::show() {
    while (window->isOpen()) {
        sync();
        sf::Event event;
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);
            switch (event.type) {
                case sf::Event::KeyReleased: key_pressed(event.key.code); break;
                case sf::Event::Closed: window->close(); break;
                default: break;
            }
            switch (tab) {
                case NAV: nav_ui->handle(&event); break;
                default: break;
            }
        }
        if (!player_sub) tab = MAINMENU; // Kein Sub? -> Hauptmenü

        // Gfx Interpolieren (nur eigenes Sub)
        if (static sf::Clock timer_interpol; player_sub) {
            player_sub->tick(nullptr, timer_interpol.getElapsedTime().asSeconds() * klient->get_timelapse());
            timer_interpol.restart();
        }

        draw_imgui();
        window->clear();
        draw_gfx();

        ImGui::SFML::Render(*window);
        window->display();
    }
}

void Spielszene::draw_imgui() {
    static sf::Clock fps_clock;
    ImGui::SFML::Update(*window, fps_clock.restart());
    // Welches Menü rendern?
    switch (tab) {
        case MAINMENU: draw_menu(); break;
        case MAP:      map_ui->update_and_show(player_sub.get());    break;
        case NAV:      nav_ui->update_and_show(player_sub.get());    break;
        case SONAR:    sonar_ui->update_and_show(player_sub.get());  break;
        case WEAPONS:  waffen_ui->update_and_show(player_sub.get()); break;
        case THREE_D:  break;
        default:       tab = MAINMENU; break;
    }
}

void Spielszene::draw_gfx() {
    switch (tab) {
        case MAINMENU: break;
        case MAP:      map_ui->draw_gfx(player_sub.get(), window);    break;
        case NAV:      nav_ui->draw_gfx(player_sub.get(), window);    break;
        case SONAR:    sonar_ui->draw_gfx(player_sub.get(), window);  break;
        case WEAPONS:  waffen_ui->draw_gfx(player_sub.get(), window); break;
        case THREE_D:  break;
        default: break;
    }
}
