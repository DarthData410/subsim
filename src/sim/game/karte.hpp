#pragma once

#include <array>
#include <cstdint>
#include <SimplexNoise.h>
#include <cereal/types/memory.hpp>

namespace sf { class Image; }

class Karte {

public:

    /// Erzeugt eine neue zufällige Karte.
    Karte();

    /// Noise-Rohwert [0.0, 1.0] @ x/y.
    float get_raw_at(float x, float y) const;

    /// Liefert die Höhe in Metern bei Position x,y.
    float get_height_at(float x, float y) const;

    /// Liefert einen RGBA-Farbwert, der die Höhe in Metern bei Position x,y repräsentiert.
    std::array<uint8_t, 4> get_rgba_at(float x, float y) const;

    /// Liefert ein Bild der Karte (mit gegebener Größe in Pixeln).
    std::unique_ptr<sf::Image> get_image(unsigned size_x, unsigned size_y) const;

    /// Liefert den nächstmöglichen passierbaren Punkt nach dem Limit `height` und dem Vergleich `cmp`.
    std::pair<float, float> get_nearest_passable(float x, float y, float height, const std::function<bool(float, float)>& cmp);

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(noise, water_level, map_shift_x, map_shift_y, octaves);
    }

private:

    SimplexNoise noise;
    float water_level;
    unsigned map_shift_x;
    unsigned map_shift_y;
    unsigned octaves;

};
