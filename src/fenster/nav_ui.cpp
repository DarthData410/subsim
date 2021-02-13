#include "nav_ui.hpp"
#include "imgui_addons.hpp"

Nav_UI::Nav_UI() : Standard_UI(nullptr) {
    //
}

Nav_UI::Nav_UI(Klient* klient) : Standard_UI(klient) {
    //
}

void Nav_UI::update_and_show(const Sub* sub) {
    (void) sub;

    ImGui::Begin("Nav");



    ImGui::End();
}
