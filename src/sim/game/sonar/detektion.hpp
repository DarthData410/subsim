#include <cstdint>

/// Einzelne Sichtung.
struct Detektion final {

    enum class Typ : uint8_t {
        MOVEMENT_SIGNATURE = 1, // TODO momentan ohne Effekt
    };

    uint32_t objekt_id; /// ID des erkannten Objekts. Kann bereits vernichtet sein.
    float gain;         /// Lautstärke der Signatur.
    float bearing;      /// Kurs.
    Typ typ;            /// Typ der Signatur.

    template <class Archive> void serialize(Archive& ar) {
        ar(objekt_id, gain, bearing, typ);
    }

};
