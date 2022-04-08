#include "../../typedefs.hpp"

#include <cereal/types/optional.hpp>

/// Einzelne Sichtung.
class Detektion final {

public:

    Detektion() = default;

    Detektion(oid_t objekt_id, float gain, float bearing, const std::optional<dist_t>& range = std::nullopt) :
        objekt_id(objekt_id), gain(gain), bearing(bearing), range(range) {}

    /*enum class Typ : uint8_t {
        MOVEMENT_SIGNATURE = 1, // TODO momentan ohne Effekt
    };*/

    oid_t objekt_id;             /// ID des erkannten Objekts. Kann bereits vernichtet sein.
    float gain;                  /// Lautstärke der Signatur.
    float bearing;               /// Kurs (absolut,d.h. nicht relativ zum Elternobjekt).
    std::optional<dist_t> range; /// Entfernung in m.
    //Typ typ;                     /// Typ der Signatur.

    template <class Archive> void serialize(Archive& ar) {
        ar(objekt_id, gain, bearing, range); // typ
    }

};
