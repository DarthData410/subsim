#include "spielszene.hpp"
#include "imgui_addons.hpp"
//#include "../gfx/image.hpp"

#include <zufall.hpp>

void Spielszene::render_nav() {
    nav_ui.update_and_show(&player_sub.value());
}

void Spielszene::render_sonar() {
    sonar_ui.update_and_show(&player_sub.value());
}

void Spielszene::render_weapons() {
    waffen_ui.update_and_show(&player_sub.value());
}

void Spielszene::render_3d() {

}

void Spielszene::render_menu() {
    float timelapse = klient->get_timelapse();
    if (ImGui::SliderFloat("Timelapse", &timelapse, 0, 10)) {
        Log::debug() << "Timelapse sollte jetzt sein: " << timelapse << '\n';
        const Kommando neue_zeit_kommando(Kommando::TIMELAPSE, 0, timelapse);
        klient->kommando(neue_zeit_kommando); // TODO nicht so oft an den server senden
    }
}
