#pragma once

#include <array>

#include "vektor.hpp"

/// Mathematische/Physikalische Hilfsfunktionen.
namespace Physik {

    /// Typ für Distanzen.
    typedef double dist_t;

    /// Typ für Winkel.
    typedef float winkel_t;

    /// Bewegt `pos` in Richtung `q` um `amount`.
    void move(Vektor& pos, winkel_t kurs, dist_t weite);

    /// Liefert den Kurs von Punkt (x,y) zu Punkt (target_x,target_y).
    winkel_t kurs(dist_t x, dist_t y, dist_t target_x, dist_t target_y);
    /// Liefert den Kurs von Punkt (x,y) zu Punkt (target_x,target_y).
    winkel_t kurs(const Vektor& v, const Vektor& v_target);

    /// Liefert die Distanz zwischen zwei 2D-Koordinaten.
    dist_t distanz(dist_t x1, dist_t y1, dist_t x2, dist_t y2);
    /// Liefert die xy-Distanz zwischen zwei 3D-Koordinaten.
    dist_t distanz_xy(const Vektor& v1, const Vektor& v2);
    /// Liefert die xyz-Distanz zwischen zwei 3D-Koordinaten.
    dist_t distanz_xyz(const Vektor& v1, const Vektor& v2);

    /// Liefert die kürzeste Rotation (in Grad) von Winkel 1 zu Winkel 2. Zwischen -180 und +180.
    winkel_t winkel_diff(winkel_t winkel1, winkel_t winkel2);

    /// Liefert den Winkel in der y-Ebene (d.h. Höhe/Tiefe) zwischen zwei 3D-Punkten.
    winkel_t winkel_tiefe(const Vektor& pos, const Vektor& target_pos);

    /// Liefert distanz(v1,v2) <= reichweite.
    bool in_reichweite_xy(const Vektor& v1, const Vektor& v2, dist_t reichweite);

    /// Liefert den Bremsweg bei gegebener Geschwindigkeit und Entschleunigung.
    dist_t bremsweg(dist_t v, dist_t a);

    /// Liefert den Punkt x,y als `std::pair` vom gegebenen Punkt aus gesehen im gg. Winkel bei gg. Entfernung.
    std::pair<dist_t, dist_t> get_punkt(dist_t x, dist_t y, winkel_t kurs, dist_t entfernung);

};
