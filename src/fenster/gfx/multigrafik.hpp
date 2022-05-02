#pragma once

#include <vector>
#include <string>

namespace sf { class RenderWindow; }
namespace gfx { class Grafik; }

namespace gfx {
class Multigrafik final {

public:

    /**
     * Erzeugt eine Multi-Frame-Grafik aus gegebenen Bildern (via Pfade) bei Position x_pos, y_pos.
     * @note Grafik ist so groß wie die Originale. Diese sollten gleich groß sein.
     */
    Multigrafik(const std::vector<std::string>& grafik_pfade, int x_pos, int y_pos);

    ~Multigrafik();

    /// Zeichnet Textur Nummer `frame` (beginnened mit 0) auf das gegebene Fenster.
    void draw(sf::RenderWindow* fenster, unsigned frame);

    /// Wurden Texturen erfolgreich geladen?
    bool good() const { return !grafiken.empty(); }

    /// Größe x.
    unsigned get_size_x() const { return size_x; }

    /// Größe y.
    unsigned get_size_y() const { return size_y; }

    /// Befindet sich gegebener Punkt innerhalb der Grafik?
    bool is_inside(float x, float y) const;

private:

    std::vector<gfx::Grafik*> grafiken;
    unsigned size_x;
    unsigned size_y;
    bool alle_grafiken_valide;

};
}
