#pragma once

#include <array>

#include "vektor.hpp"

/// Mathematische/Physikalische Hilfsfunktionen.
namespace Physik {

    /// Bewegt `pos` in Richtung `q` um `amount`.
    void move(Vektor& pos, float kurs, double weite);

    /// Liefert den Kurs von Punkt (x,y) zu Punkt (target_x,target_y).
    float kurs(float x, float y, float target_x, float target_y);

    /// Liefert die kürzeste Rotation (in Grad) von Winkel 1 zu Winkel 2. Zwischen -180 und +180.
    float winkel_diff(float winkel1, float winkel2);

    /// Liefert den Winkel in der y-Ebene (d.h. Höhe/Tiefe) zwischen zwei 3D-Punkten.
    float winkel_tiefe(const Vektor& pos, const Vektor& target_pos);

    /// Liefert die Distanz zwischen zwei 2D-Koordinaten.
    double distanz(double x1, double y1, double x2, double y2);

    /// Liefert die xy-Distanz zwischen zwei 3D-Koordinaten.
    double distanz_xy(const Vektor& v1, const Vektor& v2);

    /// Liefert die xyz-Distanz zwischen zwei 3D-Koordinaten.
    double distanz_xyz(const Vektor& v1, const Vektor& v2);

    /// Liefert distanz(v1,v2) <= reichweite.
    bool in_reichweite_xy(const Vektor& v1, const Vektor& v2, double reichweite);

    /// Liefert den Bremsweg bei gegebener Geschwindigkeit und Entschleunigung.
    double bremsweg(double v, double a);

    /// Liefert den Punkt x,y als `std::pair` vom gegebenen Punkt aus gesehen im gg. Winkel bei gg. Entfernung.
    std::pair<double, double> get_punkt(double x, double y, float kurs, double entfernung);

};
