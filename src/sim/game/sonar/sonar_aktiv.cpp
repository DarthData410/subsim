#include <zufall.hpp>
#include "sonar_aktiv.hpp"
#include "../objekte/ping.hpp"
#include "../../welt.hpp"
#include "../../physik.hpp"

Sonar_Aktiv::Sonar_Aktiv(const std::string& name, Groesse groesse, float resolution, uint16_t resolution_range,
                         dist_t max_range, float ping_intervall_min,
                         const std::vector<std::tuple<float, float>>& blindspots) :
    Sonar(name, groesse, resolution, blindspots),
    mode(Mode::OFF), max_range(max_range), resolution_range(resolution_range),
    ping_intervall_min(ping_intervall_min), ping_counter(0)
{
    intervall = std::max(ping_intervall_min, 10.f); // Standardeinstellung: x oder Min-Intervall
}

void Sonar_Aktiv::tick(Objekt* parent, Welt* welt, float s) {
    // Detektionen überhaupt durchführen?
    timer += s;
    if (this->mode == Mode::OFF) return; // Eingeschaltet?
    if (timer < intervall) return; // Ist es an der Zeit?
    if (this->mode == Mode::SINGLE) mode = Mode::OFF; // nur 1x
    detektionen.clear();
    timer = 0;

    // Ping!
    Ping* ping = new Ping(parent, max_range);
    welt->add(ping);
    ++ping_counter;

    // Detektionen berechnen
    static const std::unordered_set<Objekt::Typ> erkennbare_typen = {
            Objekt::Typ::SUB,
            Objekt::Typ::SUB_AI,
            Objekt::Typ::DECOY,
    };
    for (const auto& paar : welt->get_objekte()) {
        const Objekt* o = paar.second.get();
        if (o == parent) continue; // eigenes Sub ignorieren
        if (erkennbare_typen.count(o->get_typ()) == 0) continue; // nicht erkennbarer Typ
        if (!Physik::in_reichweite_xyz(parent->get_pos(), o->get_pos(), this->max_range)) continue; // zu weit
        if (is_in_toter_winkel(Physik::kurs_relativ(parent, o))) continue; // im toten Winkel

        // Erkannt!
        const winkel_t kurs = Physik::kurs(parent->get_pos(), o->get_pos());
        const dist_t entfernung = Physik::distanz_xyz(parent->get_pos(), o->get_pos());
        detektionen.emplace_back(Detektion(
                o->get_id(),
                Detektion::Typ::ACTIVE_SONAR_ECHO,
                o->get_noise(), // TODO gut so oder immer 1.0f einfach? Oder mal Physik::schallfaktor(entfernung) ?
                Physik::round(kurs, this->resolution),
                entfernung + Zufall::f(-0.5f * this->resolution_range, 0.5f * this->resolution_range),
                o->get_pos().z()
        ));
    }
}
