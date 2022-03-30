#include "sonar_passiv.hpp"
#include "../objects/sub.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <cmath>

Sonar_Passiv::Sonar_Passiv(float noise, uint16_t resolution, uint8_t sichtbereich)
    : noise(noise), resolution(resolution), sichtbereich(sichtbereich), ausrichtung(0)
{
    //
}

void Sonar_Passiv::tick(Sub* parent, Welt* welt, float s) {
    // Erkennungen auffrischen
    detections.clear();
    for (const auto& objekt_paar : welt->objekte) {
        const Objekt* objekt = objekt_paar.second;

        // Eigenes Sub überspringen.
        if (parent == objekt) continue;

        const float sub_bearing = parent->get_bearing();
        const float sonar_bearing = Physik::winkel_diff(0.f, sub_bearing + this->ausrichtung);
        const float object_bearing = Physik::kurs(parent->get_pos().x(), parent->get_pos().y(),
                                                  objekt->get_pos().x(), objekt->get_pos().y());
        const float sonar_to_object = Physik::winkel_diff(sonar_bearing, object_bearing);

        // Nihcht im Sichtbereich?
        if (std::abs(sonar_to_object) > static_cast<float>(this->sichtbereich) / 2.f) continue;

        // Lautstärke bestimmen
        const float distance = Physik::distanz_xyz(parent->get_pos(), objekt->get_pos());
        const float object_noise = objekt->get_noise();
        const float gain = distance > 1.f ? object_noise / std::pow(distance, 0.25f) // TODO adjust
                                          : object_noise;
        // Detektion!
        if (gain > 0.f) {
            const Detektion d = {
                    .objekt_id = objekt->get_id(),
                    .gain = gain,
                    .bearing = object_bearing,
                    .typ = Detektion::MOVEMENT_SIGNATURE
            };
            detections.push_back(d);
        }
    }
}
