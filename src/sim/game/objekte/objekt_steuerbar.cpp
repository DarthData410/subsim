#include <log.hpp>
#include <valarray>
#include "objekt_steuerbar.hpp"
#include "../../physik.hpp"

Objekt_Steuerbar::Objekt_Steuerbar(const Vektor& pos,
                                   const Motor& motor_linear,
                                   const Motor& motor_rot,
                                   const Motor& motor_tauch,
                                   float noise)
        : Objekt(pos),
          motor_linear(motor_linear), motor_rot(motor_rot), motor_tauch(motor_tauch),
          noise(noise)
{
    //
}

void Objekt_Steuerbar::stop() {
    motor_linear.v_target = 0;
    motor_rot.v_target    = 0;
    motor_tauch.v_target  = 0;
    target_bearing = std::nullopt;
    target_pos     = std::nullopt;
    target_depth   = std::nullopt;
}

bool Objekt_Steuerbar::tick(Welt* welt, float s) {
    (void) welt;

    // Automatisches Pfadfinden
    if (target_pos)     auto_path();
    if (target_bearing) auto_rudder();
    if (target_depth)   auto_depth();

    // Beschleunigen/Bremsen
    motor_linear.tick(s);
    motor_tauch.tick(s);
    motor_rot.tick(s);

    // Links/Rechts in Grad bewegen
    // + Max. Rotation ist Geschwindigkeitsabhängig
    static constexpr float eps = 0.0001f;
    if (std::abs(motor_rot.v) > eps) {
        if (std::abs(motor_rot.v) > motor_rot.v_max * get_speedfaktor()) motor_rot.v = motor_rot.v_max * get_speedfaktor();
        kurs += get_speed_rot() * s;
    }

    // Höhenbewegung
    if (std::abs(motor_tauch.v) > eps) {
        const float tauch_faktor = std::max(0.25f, get_speedfaktor());
        if (std::abs(motor_tauch.v) > motor_tauch.v_max * tauch_faktor) {
            motor_tauch.v = motor_tauch.v > 0 ? (motor_tauch.v_max * tauch_faktor) : (-motor_tauch.v_max * tauch_faktor);
        }
        pos.z(pos.z() + (get_speed_tauch() * s));
    }

    // Vorwärtsbewegung ausführen
    if (std::abs(motor_linear.v) > eps) Physik::move(pos, kurs, motor_linear.v * s);

    // noch am Leben?
    return schaeden.count(Schaden::ZERSTOERT) == 0;
}

void Objekt_Steuerbar::auto_rudder() {
    const float rotate_to = Physik::winkel_diff(get_bearing(), target_bearing.value());
    if (std::abs(rotate_to) <= motor_rot.get_bremsweg()) { // Erledigt -> Bremsen
        motor_rot.v_target = 0;
        target_bearing = std::nullopt;
    }
    else if (rotate_to > 0) motor_rot.v_target =  motor_rot.v_max;
    else if (rotate_to < 0) motor_rot.v_target = -motor_rot.v_max;
}

void Objekt_Steuerbar::auto_depth() {
    const float target_depth_left = target_depth.value() - (float)pos.z();
    const auto bremsweg = motor_tauch.get_bremsweg();
    if (std::abs(target_depth_left) <= bremsweg * 10.f) { // Tauchvorgang verlangsamen
        motor_tauch.v_target = 0.5f * motor_tauch.v_max;
        if (std::abs(target_depth_left) <= bremsweg) { // Tiefe erreicht -> Stop
            motor_tauch.v_target = 0;
            target_depth = std::nullopt;
        }
    }
    else if (target_depth_left > 0) motor_tauch.v_target =  motor_tauch.v_max;
    else if (target_depth_left < 0) motor_tauch.v_target = -motor_tauch.v_max;
}

void Objekt_Steuerbar::auto_path() {
    // Richtung
    const auto target_x = target_pos.value()[0];
    const auto target_y = target_pos.value()[1];
    const auto bearing = Physik::kurs(pos.x(), pos.y(), target_x, target_y);
    if (std::abs(get_bearing() - bearing) > 1.f) set_target_bearing(bearing);
    if (Physik::distanz(pos.x(), pos.y(), target_x, target_y) <= motor_linear.get_bremsweg()) {
        target_pos = std::nullopt; // Ziel erreicht.
        set_target_bearing(bearing);
        stop();
    }
}

float Objekt_Steuerbar::get_speedfaktor() const {
    return std::sqrt(std::abs(get_speed_relativ()));
}

bool Objekt_Steuerbar::apply_damage(const Explosion* explosion, float damage) {
    (void) explosion;
    if (damage <= 0) return false;
    // Totalschaden
    if (schaeden.count(Schaden::ZERSTOERT)) return false; // war bereits zerstört
    Log::debug() << "Objekt_Steuerbar " << this->get_id() << " Typ=" << (int)this->get_typ() << " zerstoert.\n";
    schaeden.insert(Schaden::ZERSTOERT);
    return true;
}

const std::string& Objekt_Steuerbar::get_name() const {
    static const std::string s("Invalid Object [B]");
    return s;
}
