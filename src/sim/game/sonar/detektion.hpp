#include "../../typedefs.hpp"

#include <cereal/types/optional.hpp>

/// Einzelne Sichtung.
class Detektion final {

public:

    enum class Typ : uint8_t {
        MOVEMENT_SIGNATURE = 1, // Antriebsgeräusch
        ACTIVE_SONAR_PING  = 2, // Einzelner Ping
        ACTIVE_SONAR_ECHO  = 3, // Objekt-Echo eines a. Sonar-Pings
    };

    Detektion() = default;

    Detektion(oid_t objekt_id, Detektion::Typ typ, float gain, float bearing,
              const std::optional<dist_t>& range = std::nullopt, const std::optional<float>& depth = std::nullopt) :
        objekt_id(objekt_id), gain(std::clamp(gain, 0.f, 1.0f)), bearing(bearing), depth(depth), range(range), typ(typ) {}

    oid_t objekt_id;             /// ID des erkannten Objekts. Kann bereits vernichtet sein.
    float gain;                  /// Lautstärke der Signatur. Garantiert im Bereich [0,1].
    float bearing;               /// Kurs (absolut; d.h. nicht relativ zum Elternobjekt).
    std::optional<float> depth;  /// Negative Werte: unterhalb der Wasseroberfläche (=Tiefe); Positiv: Darüber (=(Flug-)Höhe).
    std::optional<dist_t> range; /// Entfernung in m.
    Typ typ;                     /// Typ der Signatur.

    template <class Archive> void serialize(Archive& ar) {
        ar(objekt_id, gain, bearing, depth, range, typ); // typ
    }

};
