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
    const float max_vm = std::sqrt(std::abs(get_speed_relativ())); // Maximale Manövergeschwindigkeit.
    if (std::abs(motor_rot.v) > motor_rot.v_max * max_vm) motor_rot.v = motor_rot.v * max_vm;
    if (std::abs(motor_rot.v) > eps) kurs += motor_rot.v * s;

    // Bewegungen ausführen
    if (std::abs(motor_linear.v) > eps) Physik::move(pos, kurs, motor_linear.v * s);
    if (std::abs(motor_tauch.v)  > eps) pos.z(pos.z() + motor_tauch.v);

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

bool Objekt_Steuerbar::apply_damage(Explosion* explosion, float damage) {
    (void) explosion;
    if (damage <= 0) return false;
    // Totalschaden
    if (schaeden.count(Schaden::ZERSTOERT)) return false; // war bereits zerstört
    Log::debug() << "Objekt " << this->get_id() << " Typ=" << (int)this->get_typ() << " zerstoert.\n";
    schaeden.insert(Schaden::ZERSTOERT); // TODO - momentan immer Zerstörung
    return true;
}

const std::string& Objekt_Steuerbar::get_name() const {
    static const std::string s("Invalid Object [B]");
    return s;
}
