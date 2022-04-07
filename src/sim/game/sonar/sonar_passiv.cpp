#include "sonar_passiv.hpp"
#include "../objekte/sub.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <utility>

Sonar_Passiv::Sonar_Passiv(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots)
    : Sonar(noise_threshold, resolution, std::move(blindspots))
{
    //
}

void Sonar_Passiv::tick(Objekt* parent, Welt* welt, float s) {
    // Zeit, Erkennungen aufzufrischen?
    timer += s;
    if (timer < detection_intervall) return;
    timer = 0;

    detektionen.clear();
    for (const auto& objekt_paar : welt->objekte) {
        const Objekt* objekt = objekt_paar.second.get();

        // Eigenes Sub überspringen.
        if (parent == objekt) continue;

        // Im Sichtbereich?
        const winkel_t kurs_relativ = Physik::kurs_relativ(parent, objekt);
        if (is_in_toter_winkel(kurs_relativ)) continue;

        // Lautstärke bestimmen
        const auto distanz = Physik::distanz_xyz(parent->get_pos(), objekt->get_pos());
        const auto sichtbarkeit = Physik::sichtbarkeit(objekt->get_noise(), objekt->get_speed(), distanz);

        // Detektion!
        if (sichtbarkeit >= this->noise) {
            const Detektion d = {
                    .objekt_id = objekt->get_id(),
                    .gain = sichtbarkeit,
                    .bearing = Physik::round(kurs_relativ, this->resolution),
                    .typ = Detektion::Typ::MOVEMENT_SIGNATURE
            };
            detektionen.emplace_back(d);
        }
    }
}
