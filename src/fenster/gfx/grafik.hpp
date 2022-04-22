#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <unordered_map>

/**
 * Wurzelklasse für alle Dinge, die vom Fenster gerendert werden.
 * Enthält zum Anzeigen eine Methode `draw`.
 * @version 1.1
 * @author nada
 */
class Grafik final {

public:

    /**
     *  Konstruktor.
     *  @note `is_shared` = true bedeutet, dass die Grafik nicht gelöscht wird nach ableben der Instanz.
     */
    Grafik() : is_shared(false), is_flipped(false) {};

    /// ctor. Erzeugt eine Grafik aus gegebener Datei (`jpg`, `png` ...). Formate: vgl SFML Doku.
    explicit Grafik(const std::string& textur_pfad, bool is_shared = false);

    /// dtor.
    virtual ~Grafik();

    /// Zeichnet die Textur auf das gegebene Fenster.
    virtual void draw(sf::RenderWindow* fenster);

    /// Getter: X-Koordinate.
    float x() const { return sprite.getPosition().x; }

    /// Getter: Y-Koordinate.
    float y() const { return sprite.getPosition().y; }

    /// Getter: Kompletter Positionsvektor.
    const sf::Vector2f& pos() const { return sprite.getPosition(); }

    /// Liegt gegebener Punkt in der Grafik?
    bool is_inside(float x, float y) const;

    /// Liefert einen Pointer zur verwendeten Textur.
    sf::Texture* data() const { return textur; }

    /// Liefert den verwendeten Sprite.
    const sf::Sprite& get_sprite() const { return sprite; }

    /// Grafik geladen?
    bool good() const { return textur != nullptr; }

    /// Liefert die Dimensionen der Textur.
    sf::Vector2u size() const { return textur->getSize(); }

    /// Liefert die x-Dimensionen der Grafik.
    float size_x() const { return textur->getSize().x * sprite.getScale().x; }

    /// Liefert die y-Dimensionen der Grafik.
    float size_y() const { return textur->getSize().y * sprite.getScale().y; }

    /// Liefert die Dimensionen der Grafik zu float konvertiert.
    sf::Vector2f size_f() const;

    /// Liefert den Pfad zur Grafik.
    const std::string& get_pfad() const { return textur_pfad; }

    /// Manipuliert (multipliziert) gegebene Farbe mit der Grafik.
    void set_color(const sf::Color& farbe) { sprite.setColor(farbe); }

    void set_opacity(float faktor);

    /// Skaliert die Grafik auf gegebene Größe in Pixel.
    void set_size(const sf::Vector2f& size);

    void set_size(float scale) { sprite.setScale(scale, scale); }

    /// Setter: X-Koordinate.
    void set_x(float x) { sprite.setPosition(x, sprite.getPosition().y); }

    /// Setter: Y-Koordinate.
    void set_y(float y) { sprite.setPosition(sprite.getPosition().x, y); }

    /// Setter: Position als Vektor.
    void set_pos(const sf::Vector2f& pos) { sprite.setPosition(pos); }

    /// Setter: Horizontal spiegeln.
    void set_flip(bool flip);

    /// Kann verwendet werden, um zu prüfen, ob eine Grafik geladen ist.
    explicit operator bool() const { return textur != nullptr; }

    /// Überprüft, ob die Grafiken dieselbe Textur besitzen.
    bool operator==(const Grafik& rhs) const { return textur == rhs.textur; }

    /// Überprüft, ob die Grafiken unterschiedliche Texturen besitzen.
    bool operator!=(const Grafik& rhs) const { return !(rhs == *this); }

private:

    bool is_shared;

    bool is_flipped;

    std::string textur_pfad;

    sf::Texture* textur = nullptr;

    sf::Sprite sprite;

};
