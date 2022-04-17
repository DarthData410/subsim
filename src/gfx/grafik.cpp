#include <iostream>
#include <sstream>
#include <fstream>
#include "grafik.hpp"

Grafik::Grafik(const std::string& textur_pfad, bool is_shared) :
        is_shared(is_shared),
        is_flipped(false),
        textur_pfad(textur_pfad)
{
    // Textur noch nicht vorhanden
    textur = new sf::Texture;
    if (textur->loadFromFile(textur_pfad)) std::cout << "\t\tGrafik erzeugt: " << textur_pfad << '\n';
    else {
        std::cerr << "Grafik() Textur " << textur_pfad << " konnte nicht geladen werden.\n";
        delete textur;
        textur = nullptr;
    }

    // Textur setzen
    if (textur) sprite.setTexture(*textur);
}

Grafik::~Grafik() {
    delete textur;
}

void Grafik::set_flip(bool flip) {
    if (flip == is_flipped) return;
    const auto& r = sprite.getTextureRect();
    if (flip) {
        sprite.setTextureRect(sf::IntRect(std::abs(r.width), 0, -std::abs(r.width), r.height));
    }
    else {
        sprite.setTextureRect(sf::IntRect(0, 0, std::abs(r.width), r.height));
    }
    is_flipped = flip;
}

void Grafik::set_size(const sf::Vector2f& size) {
    sprite.setScale(size.x / textur->getSize().x, size.y / textur->getSize().y);
}

sf::Vector2f Grafik::size_f() const {
    return sf::Vector2f(size().x, size().y);
}

bool Grafik::is_inside(float x, float y) const {
    return sprite.getGlobalBounds().contains(x, y);
}

void Grafik::draw(sf::RenderWindow* fenster) {
    fenster->draw(sprite);
}

void Grafik::set_opacity(float faktor) {
    auto color = sprite.getColor();
    color.a = std::clamp(int(0xFF * faktor), 0, 0xFF);
    sprite.setColor(color);
}
