#include "grafik.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <iostream>
#include <memory>

gfx::Grafik::Grafik() : is_shared(false), is_flipped(false) {}
gfx::Grafik::Grafik(const std::string& textur_pfad, bool is_shared) :
        is_shared(is_shared), // TODO
        is_flipped(false),
        textur_pfad(textur_pfad),
        textur(nullptr),
        rect(nullptr)
{
#ifndef UNITTEST // Damit Unittests auch ohne z.B. X11 laufen
    // Textur noch nicht vorhanden
    textur = new sf::Texture;
    if (textur->loadFromFile(textur_pfad)) std::cout << "\t\tGrafik erzeugt: " << textur_pfad << '\n';
    else {
        std::cerr << "Grafik() Textur " << textur_pfad << " konnte nicht geladen werden.\n";
        textur = nullptr;
    }
    // Textur setzen
    if (textur) {
        rect = new sf::RectangleShape;
        rect->setSize({static_cast<float>(textur->getSize().x), static_cast<float>(textur->getSize().y)});
        rect->setTexture(textur);
    }
#else
    textur = nullptr;
#endif
}

gfx::Grafik::~Grafik() {
    delete rect;
    delete textur;
}

void gfx::Grafik::set_flip(bool flip) {
    if (flip == is_flipped) return;
    const auto& r = rect->getTextureRect();
    if (flip) rect->setTextureRect(sf::IntRect(std::abs(r.width), 0, -std::abs(r.width), r.height));
    else rect->setTextureRect(sf::IntRect(0, 0, std::abs(r.width), r.height));
    is_flipped = flip;
}

sf::Vector2u gfx::Grafik::size() const {
    return textur->getSize();
}

void gfx::Grafik::set_size(const sf::Vector2f& size) {
    rect->setScale(size.x / textur->getSize().x, size.y / textur->getSize().y);
}

sf::Vector2f gfx::Grafik::size_f() const {
    return sf::Vector2f(size().x, size().y);
}

bool gfx::Grafik::is_inside(float x, float y) const {
    return rect->getGlobalBounds().contains(x, y);
}

void gfx::Grafik::draw(sf::RenderWindow* fenster) {
    fenster->draw(*rect);
}

void gfx::Grafik::set_opacity(float faktor) {
    auto color = rect->getFillColor();
    color.a = std::clamp(int(0xFF * faktor), 0, 0xFF);
    rect->setFillColor(color);
}

float gfx::Grafik::size_x() const { return textur->getSize().x * rect->getScale().x; }

float gfx::Grafik::size_y() const { return textur->getSize().y * rect->getScale().y; }

void gfx::Grafik::set_pos(const sf::Vector2f& pos) { rect->setPosition(pos); }

void gfx::Grafik::set_y(float y) { rect->setPosition(rect->getPosition().x, y); }

void gfx::Grafik::set_x(float x) { rect->setPosition(x, rect->getPosition().y); }

void gfx::Grafik::set_size(float scale) { rect->setScale(scale, scale); }

void gfx::Grafik::set_color(const sf::Color& farbe) { rect->setFillColor(farbe); }

const sf::Vector2f& gfx::Grafik::pos() const { return rect->getPosition(); }

float gfx::Grafik::x() const { return rect->getPosition().x; }

float gfx::Grafik::y() const { return rect->getPosition().y; }

sf::Texture* gfx::Grafik::data() const { return textur; }
