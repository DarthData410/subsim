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

void Spielszene::sync() {
    // Net Sync
    if (static sf::Clock timer; timer.getElapsedTime().asMilliseconds() >= 500) {
        if (player_sub) {
            const std::string& antwort = klient->request(Net::REQUEST_SUB, player_sub->get_id());
            if (!antwort.empty()) {
                player_sub = Net::deserialize<Sub>(antwort);
                //Log::debug() << "sync sub id=" << player_sub->get_id() << '\n';
            }
            else Log::err() << "Spielszene::" << __func__ << " no sub returned with ID " << player_sub->get_id() << '\n';
        }
        timer.restart();
    }
}

void Spielszene::key_pressed(const sf::Keyboard::Key& key) {
    switch (key) {
        case sf::Keyboard::Escape: window->close(); break;

        // Neues Spieler Sub geben (nur wenn keins vorhanden)
        case sf::Keyboard::M: {
            if (player_sub) { Log::debug() << "New player_sub not needed\n"; break; }
            const std::string& antwort = klient->request(Net::AKTION_NEUES_UBOOT, 1); // objekt_id = Team
            if (!antwort.empty()) {
                player_sub = Net::deserialize<Sub>(antwort);
                nav_ui     = Nav_UI(klient.get());
                sonar_ui   = Sonar_UI(klient.get());
                waffen_ui  = Waffen_UI(klient.get());
                Log::debug() << "New player_sub id=" << player_sub->get_id() << '\n';
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

void Spielszene::draw_menu() {
    // Timelapse
    ImGui::Text("Server Timelapse = %.1f", klient->get_timelapse());
    static float timelapse = 1;
    ImGui::SliderFloat("Timelapse", &timelapse, 0, 20, "%.1f");
    if (ImGui::SameLine(); ImGui::Button("Set")) {
        Log::debug() << "Timelapse sollte jetzt sein: " << timelapse << '\n';
        const Kommando neue_zeit_kommando(Kommando::TIMELAPSE, 0, timelapse);
        klient->kommando(neue_zeit_kommando);
    }
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
                case NAV: nav_ui.handle(&event); break;
                default: break;
            }
        }
        if (!player_sub) tab = MAINMENU; // Kein Sub? -> Hauptmenü

        // Gfx Interpolieren (nur eigenes Sub)
        if (static sf::Clock timer_interpol; player_sub.has_value()) {
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
    ImGui::Begin("Spielszene");
    switch (tab) {
        case MAINMENU: draw_menu(); break;
        case NAV:      nav_ui.update_and_show(&player_sub.value());    break;
        case SONAR:    sonar_ui.update_and_show(&player_sub.value());  break;
        case WEAPONS:  waffen_ui.update_and_show(&player_sub.value()); break;
        case THREE_D:  break;
        default:       tab = MAINMENU; break;
    }
    ImGui::End();
}

void Spielszene::draw_gfx() {
    switch (tab) {
        case MAINMENU: break;
        case NAV:      nav_ui.draw_gfx(&player_sub.value(), window); break;
        case SONAR:    sonar_ui.draw_gfx(&player_sub.value(), window); break;
        case WEAPONS:  waffen_ui.draw_gfx(&player_sub.value(), window); break;
        case THREE_D:  break;
        default: break;
    }
}
