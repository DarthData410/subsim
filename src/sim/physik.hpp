#pragma once

#include <array>

#include "vektor.hpp"
#include "typedefs.hpp"

class Objekt;

/// Mathematische/Physikalische Hilfsfunktionen.
namespace Physik {

    /// Rundet `wert` auf `faktor`. (18, 5) ergibt bspw. 20. @note Nur für Gleitkommazahlen geeignet.
    template<typename T> T round(T wert, T faktor);

    /// Bewegt `pos` in Richtung `q` um `amount`.
    void move(Vektor& pos, winkel_t kurs, dist_t weite);
    /// Liefert den Punkt x,y als `std::pair` vom gegebenen Punkt aus gesehen im gg. Winkel bei gg. Entfernung.
    std::pair<dist_t, dist_t> get_punkt(dist_t x, dist_t y, winkel_t kurs, dist_t entfernung);

    /// Liefert den Kurs von Punkt (x,y) zu Punkt (target_x,target_y).
    winkel_t kurs(dist_t x, dist_t y, dist_t target_x, dist_t target_y);
    /// Liefert den Kurs von Punkt (x,y) zu Punkt (target_x,target_y).
    winkel_t kurs(const Vektor& v, const Vektor& v_target);
    /// Liefert den relativen Kurs von Objekt1 zu Objekt2.
    winkel_t kurs_relativ(const Objekt* o1, const Objekt* o2);

    /// Liefert die Distanz zwischen zwei 2D-Koordinaten.
    dist_t distanz(dist_t x1, dist_t y1, dist_t x2, dist_t y2);
    /// Liefert die xy-Distanz zwischen zwei 3D-Koordinaten.
    dist_t distanz_xy(const Vektor& v1, const Vektor& v2);
    /// Liefert die xyz-Distanz zwischen zwei 3D-Koordinaten.
    dist_t distanz_xyz(const Vektor& v1, const Vektor& v2);
    /// Liefert distanz(v1,v2) <= reichweite.
    bool in_reichweite_xyz(const Vektor& v1, const Vektor& v2, dist_t reichweite);
    /// Liefert den Bremsweg bei gegebener Geschwindigkeit und Entschleunigung.
    dist_t bremsweg(dist_t v, dist_t a);

    /// Liefert die kürzeste Rotation (in Grad) von Winkel 1 zu Winkel 2. Zwischen -180 und +180.
    winkel_t winkel_diff(winkel_t winkel1, winkel_t winkel2);
    /// Liefert den Winkel in der y-Ebene (d.h. Höhe/Tiefe) zwischen zwei 3D-Punkten.
    winkel_t winkel_tiefe(const Vektor& pos, const Vektor& target_pos);
    /// Normalisiert gegebenen Winkel, sodass -180 <= winkel <= +180.
    winkel_t winkel_norm(winkel_t winkel);
    /// Prüft, ob `winkel` innerhalb von `min` und `max` liegt (einschließend).
    bool is_winkel_zwischen(winkel_t winkel, winkel_t min, winkel_t max);

    /// Liefert die Geräuschverringung nach Distanz (0.0 - kein Schall übrig - 1.0).
    float schallfaktor(dist_t distanz);

    /**
     * Liefert die "Sichtbarkeit" (also Geräuschentwicklung) eines Subs von 0.0 (unsichtbar) - 1.0 (ganz deutlich).
     * @param sichtbarkeitsfaktor 0.0 - 1.0 (ja nach Technologie; kleiner ist besser).
     * @param v Geschwindigkeit
     * @param d Entfernung
     * @return 0.1 und weniger bedeutet quasi unsichtbar. Absolut 0 wird praktisch nie erreicht.
     */
    float sichtbarkeit(float sichtbarkeitsfaktor, float v, dist_t d);

};
