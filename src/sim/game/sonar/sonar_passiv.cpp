#include "sonar_passiv.hpp"
#include "../objekte/sub.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

#include <utility>

Sonar_Passiv::Sonar_Passiv(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots) :
    Sonar(resolution, std::move(blindspots)),
    noise(noise_threshold)
{
    //
}

void Sonar_Passiv::tick(Objekt* parent, Welt* welt, float s) {
    // Zeit, Erkennungen aufzufrischen?
    if (timer += s; timer < detection_intervall) return;
    detektionen.clear();
    timer = 0;

    for (const auto& objekt_paar : welt->objekte) {
        const Objekt* objekt = objekt_paar.second.get();

        // Eigenes Sub überspringen.
        if (parent == objekt) continue;

        // Im toten Winkel?
        const winkel_t kurs_relativ = Physik::kurs_relativ(parent, objekt);
        if (is_in_toter_winkel(kurs_relativ)) continue;

        // Lautstärke bestimmen
        const auto distanz = Physik::distanz_xyz(parent->get_pos(), objekt->get_pos());
        const auto sichtbarkeit = Physik::sichtbarkeit(objekt->get_noise(), objekt->get_speed(), distanz);

        // Detektion!
        const auto kurs_absolut = Physik::kurs(parent->get_pos(), objekt->get_pos());
        if (sichtbarkeit >= this->noise) {
            detektionen.push_back(Detektion(
                    objekt->get_id(),
                    sichtbarkeit,
                    Physik::round(kurs_absolut, this->resolution)
            ));
        }
    }
}
