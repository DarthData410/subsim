#include "sonar_passiv.hpp"
#include "../objekte/sub.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <cmath>
#include <utility>

Sonar_Passiv::Sonar_Passiv(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots)
    : timer(0), noise(noise_threshold), resolution(resolution), blindspots(std::move(blindspots))
{
    //
}

void Sonar_Passiv::tick(Objekt* parent, Welt* welt, float s) {
    // Zeit, Erkennungen aufzufrischen?
    timer += s;
    if (s < 1.f) return;
    s = 0;

    detections.clear();
    for (const auto& objekt_paar : welt->objekte) {
        const Objekt* objekt = objekt_paar.second.get();

        // Eigenes Sub überspringen.
        if (parent == objekt) continue;

        // Im Sichtbereich?
        const winkel_t kurs_relativ = Physik::kurs_relativ(parent, objekt);
        if (is_in_blindspot(kurs_relativ)) continue;

        // Lautstärke bestimmen
        const auto distanz = Physik::distanz_xyz(parent->get_pos(), objekt->get_pos());
        const auto sichtbarkeit = Physik::sichtbarkeit(objekt->get_noise(), objekt->get_speed(), distanz);

        // Detektion!
        if (sichtbarkeit >= this->noise) {
            const Detektion d = {
                    .objekt_id = objekt->get_id(),
                    .gain = sichtbarkeit,
                    .bearing = Physik::round(kurs_relativ, this->resolution),
                    .typ = Detektion::MOVEMENT_SIGNATURE
            };
            detections.emplace_back(d);
        }
    }
}

bool Sonar_Passiv::is_in_blindspot(winkel_t kurs_relativ) const {
    return std::any_of(blindspots.begin(), blindspots.end(), [&](const auto& blindspot) {
        return Physik::is_winkel_zwischen(kurs_relativ, std::get<0>(blindspot), std::get<0>(blindspot));
    });
}
