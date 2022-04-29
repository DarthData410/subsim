#pragma once

#include <array>
#include <cstdint>
#include <cereal/types/memory.hpp>

class SimplexNoise;
namespace sf { class Image; }

class Karte {

public:

    /// Erzeugt eine neue zufällige Karte.
    Karte();

    /// Noise-Rohwert [0.0, 1.0] @ x/y.
    float get_raw_at(float x, float y) const;

    float get_height_at(float x, float y) const;

    std::array<uint8_t, 4> get_rgba_at(float x, float y) const;

    std::unique_ptr<sf::Image> get_image(unsigned size_x, unsigned size_y) const;

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(noise, water_level, map_shift_x, map_shift_y, octaves);
    }

private:

    std::unique_ptr<SimplexNoise> noise;
    float water_level;
    unsigned map_shift_x;
    unsigned map_shift_y;
    unsigned octaves;

};
