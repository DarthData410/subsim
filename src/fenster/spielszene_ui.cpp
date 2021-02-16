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

}
