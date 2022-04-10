#include "sonar_passiv.hpp"
#include "../objekte/sub.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"
#include "../objekte/ping.hpp"

#include <utility>

Sonar_Passiv::Sonar_Passiv(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots) :
    Sonar(resolution, std::move(blindspots)),
    noise(noise_threshold)
{
    //
}

void Sonar_Passiv::tick(Objekt* parent, Welt* welt, float s) {
    // Zeit, Erkennungen aufzufrischen?
    if (timer += s; timer < intervall) return;
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
        const auto o_typ = objekt->get_typ();
        const auto distanz = Physik::distanz_xyz(parent->get_pos(), objekt->get_pos());
        float sichtbarkeit;
        if (o_typ == Objekt::Typ::PING) sichtbarkeit = ((const Ping*) objekt)->get_noise_relative(distanz);
        else sichtbarkeit = Physik::sichtbarkeit(objekt->get_noise(), objekt->get_speed(), distanz);

        // Detektion!
        const auto kurs_absolut = Physik::kurs(parent->get_pos(), objekt->get_pos());
        if (sichtbarkeit >= this->noise) {
            const Detektion::Typ typ = o_typ == Objekt::Typ::PING ?
                                       Detektion::Typ::ACTIVE_SONAR_PING : Detektion::Typ::MOVEMENT_SIGNATURE;
            detektionen.push_back(Detektion(
                    objekt->get_id(),
                    typ,
                    sichtbarkeit,
                    Physik::round(kurs_absolut, this->resolution)
            ));
        }
    }
}
