#include "szene.hpp"

#include <iostream>
#include <memory>
#include <imgui-SFML.h>

Szene::Szene() :
        window(sf::VideoMode::getDesktopMode(), "<3", sf::Style::None)
{
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);
}

Szene::~Szene() {
    ImGui::SFML::Shutdown();
}

void Szene::show() {
    std::unique_ptr<Spielszene> spielszene(new Spielszene(&window));
    spielszene->show();
}
