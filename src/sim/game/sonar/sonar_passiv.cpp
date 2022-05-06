#include "sonar_passiv.hpp"
#include "../objekte/sub.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"
#include "../objekte/ping.hpp"

#include <utility>

Sonar_Passiv::Sonar_Passiv(const std::string& name, Groesse groesse, float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots) :
    Sonar(name, groesse, resolution, std::move(blindspots)),
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

        // Lautstärke bestimmen (aka noise, aka sichtbarkeit)
        const auto o_typ = objekt->get_typ();
        const auto distanz = Physik::distanz_xyz(parent->get_pos(), objekt->get_pos());
        float sichtbarkeit = 0;
        switch (o_typ) { // Berechnung der Lautstärke ist abhängig von Quell-Objekttyp
            case Objekt::Typ::PING: {
                const Ping* ping = (const Ping*) objekt;
                if (ping->get_quelle() == parent->get_id()) continue; // Ping aus eigener Quelle
                sichtbarkeit = ping->get_noise_relative(distanz);
                } break;
            case Objekt::Typ::DECOY:
            case Objekt::Typ::EXPLOSION:
                sichtbarkeit = Physik::schallfaktor(distanz) * objekt->get_noise();
                break;
            case Objekt::Typ::TORPEDO:
            case Objekt::Typ::SUB:
            case Objekt::Typ::SUB_AI:
                sichtbarkeit = Physik::sichtbarkeit(objekt->get_noise(), objekt->get_speed(), distanz);
                break;
            case Objekt::Typ::OBJEKT:
            case Objekt::Typ::OBJEKT_STEUERBAR:
                throw std::runtime_error("World contains illegal object type=" + std::to_string((int)o_typ));
        }

        // Detektion!
        const auto kurs_absolut = Physik::kurs(parent->get_pos(), objekt->get_pos());
        if (sichtbarkeit >= this->noise) { // TODO noise eigenes Sub beeinflusst das
            const Detektion::Typ typ = o_typ == Objekt::Typ::PING ? Detektion::Typ::ACTIVE_SONAR_PING :
                                       Detektion::Typ::MOVEMENT_SIGNATURE;
            detektionen.emplace_back(Detektion(
                    objekt->get_id(),
                    typ,
                    sichtbarkeit,
                    Physik::round(kurs_absolut, this->resolution),
                    objekt->get_team()
            ));
        }
    }
}
