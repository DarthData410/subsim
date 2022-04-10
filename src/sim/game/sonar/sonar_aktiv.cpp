#include "sonar_aktiv.hpp"
#include "../objekte/ping.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

Sonar_Aktiv::Sonar_Aktiv(float resolution, dist_t max_range, float ping_intervall_min,
                         const std::vector<std::tuple<float, float>>& blindspots) :
    Sonar(resolution, blindspots),
    mode(Mode::OFF), max_range(max_range), ping_intervall_min(ping_intervall_min)
{

}

void Sonar_Aktiv::tick(Objekt* parent, Welt* welt, float s) {
    // Detektionen überhaupt durchführen?
    if (this->mode == Mode::OFF) return; // Eingeschaltet?
    if (timer += s; timer < intervall) return; // Ist es an der Zeit?
    if (this->mode == Mode::SINGLE) mode == Mode::OFF; // nur 1x
    detektionen.clear();
    timer = 0;

    // Ping!
    Ping* ping = new Ping(parent, max_range);
    welt->add(ping);

    // Detektionen berechnen
    static const std::unordered_set<Objekt::Typ> erkennbare_typen = {
            Objekt::Typ::SUB,
            Objekt::Typ::SUB_AI
    };
    for (const auto& paar : welt->get_objekte()) {
        const Objekt* o = paar.second.get();
        if (o == parent) continue; // eigenes Sub ignorieren
        if (erkennbare_typen.count(o->get_typ()) == 0) continue; // nicht erkennbarer Typ
        if (!Physik::in_reichweite_xyz(parent->get_pos(), o->get_pos(), this->max_range)) continue; // zu weit

        // Erkannt!
        const winkel_t kurs = Physik::kurs(parent->get_pos(), o->get_pos());
        const dist_t entfernung = Physik::distanz_xyz(parent->get_pos(), o->get_pos());
        detektionen.push_back(Detektion(
                o->get_id(),
                Detektion::Typ::ACTIVE_SONAR_ECHO,
                1.0, // Aktiver Sonar: immer voll Sichtbar
                Physik::round(kurs, this->resolution),
                entfernung)
        );
    }
}
