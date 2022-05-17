#include "karte.hpp"
#include "../physik.hpp"

#include <SimplexNoise.h>
#include <memory>
#include <nada/random.hpp>
#include <SFML/Graphics/Image.hpp>

Karte::Karte() {
    water_level = 0.55f; // (0,1)
    octaves = 3;
    map_shift_x = nada::random::ui(0,1000000);
    map_shift_y = nada::random::ui(0,1000000);
    const float frequency   = 0.00001f;
    const float amplitude   = 0.5f;
    const float lacunarity  = 1.99f;
    const float persistence = 0.5f;
    noise = SimplexNoise(frequency, amplitude, lacunarity, persistence);
}

float Karte::get_height_at(float x, float y) const {
    return (get_raw_at(x, y) - water_level) * 10000.f;
}

float Karte::get_raw_at(float x, float y) const {
    // Von [-1.0,1.0] nach [0.0,1.0] konvertieren
    const auto val = 0.5f * (1.f + noise.fractal(octaves, x+map_shift_x, y+map_shift_y));
    return val;
}

std::array<uint8_t, 4> Karte::get_rgba_at(float x, float y) const {
    const auto noise_at_xy = get_raw_at(x, y);
    const bool is_land = noise_at_xy - water_level > 0;
    const uint8_t hv = std::clamp((int)(noise_at_xy * 0xFF), 0, 0xFF);
    return std::array<uint8_t, 4>{
            0u,
            static_cast<uint8_t>(is_land ? hv : 0),
            static_cast<uint8_t>(is_land ? 0 : hv),
            0xFFu,
    };
}

std::unique_ptr<sf::Image> Karte::get_image(unsigned int map_size_x, unsigned int map_size_y) const {
    std::unique_ptr<sf::Image> img(new sf::Image);
    img->create(map_size_x, map_size_y);
    for (unsigned y = 0; y < map_size_y; ++y) for (unsigned x = 0; x < map_size_x; ++x) {
            const auto rgba = get_rgba_at(x, y);
            img->setPixel(x, y, {rgba[0], rgba[1], rgba[2], rgba[3]});
    }
    return img;
}

std::pair<float, float>
Karte::get_nearest_passable(float x, float y, float height, const std::function<bool(float, float)>& cmp) {
    unsigned i = 0;
    if (cmp(get_height_at(x, y), height)) return {x, y};
    for (float r = 100; ; r += 100) for (float kurs = 0.f; kurs < 355.f; kurs += 45.f) {
        i++;
        const auto [px, py] = Physik::get_punkt(x, y, kurs, r);
        if (cmp(get_height_at(px, py), height)) {
            nada::Log::debug() << "Karte::get_nearest_passable " << i
                               << " Iterationen, Distanz: " << (int)Physik::distanz(x,y, px,py) << '\n';
            return {px, py};
        }
    }
}
