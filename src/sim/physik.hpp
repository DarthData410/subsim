#pragma once

#include <array>

#include "vektor.hpp"

/// Mathematische/Physikalische Hilfsfunktionen.
class Physik final {

public:

    /// Bewegt `pos` in Richtung `q` um `amount`.
    static void move_ahead(Vektor& pos, double bearing, double pitch, float speed);

    /// Liefert die kürzeste Rotation (in Grad) von Winkel 1 zu Winkel 2. Zwischen -180 und +180.
    static float rotation(float winkel1, float winkel2);

    /// Liefert den Winkel von Punkt (x,y) zu Punkt (target_x,target_y).
    static float bearing(float x, float y, float target_x, float target_y);

    /// Liefert den Winkel in der y-Ebene (d.h. Höhe/Tiefe) zwischen zwei 3D-Punkten.
    static float winkel_tiefe(const Vektor& pos, const Vektor& target_pos);

    /// Liefert die Distanz zwischen zwei 2D-Koordinaten.
    static double distanz(float x1, float y1, float x2, float y2);

    /// Liefert die Distanz zwischen zwei 2D-Koordinaten.
    static double distanz_xy(const Vektor& v1, const Vektor& v2);

    /// Liefert die Distanz zwischen zwei 3D-Koordinaten.
    static double distanz(const Vektor& v1, const Vektor& v2);

    /// Liefert den Bremsweg bei gegebener Geschwindigkeit und Entschleunigung.
    static double bremsweg(float v, float a);

    /// Liefert den Punkt x,y als `std::pair` vom gegebenen Punkt aus gesehen im gg. Winkel bei gg. Entfernung.
    static std::array<float,2> get_punkt(float x, float y, float bearing, float entfernung);

};
