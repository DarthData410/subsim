#include <Ogre.h>
#include <cereal/cereal.hpp>

class Welt;

/// Elternklasse für alle von der Physiksimulation betroffenen Objekte.
class Objekt {

public:

    /// Vererbungshierachie von Objekt. Zur Typenbestimmung bei Laufzeit.
    enum class Typ { OBJEKT, OBJEKT_STEUERBAR, SUB, TORPEDO };

    Objekt() = default;

    explicit Objekt(const Ogre::Vector3& pos, const Ogre::Quaternion& orientation =
                    Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_Y));

    virtual Typ get_typ() const = 0;

    virtual void tick(Welt* welt, float s) = 0;

    /// Getter: Einmalige ID, global gültig für alle Objekte und vererbte Klassen.
    uint32_t get_id() const { return id; }

    /// Getter: Teamzugehörigkeit. 0 = Kein Team.
    uint8_t get_team() const { return team; }

    const Ogre::Vector3& get_pos() const { return pos; }

    const Ogre::Quaternion& get_orientation() const { return orientation; }

    float get_bearing() const;

    float get_pitch() const;

    virtual ~Objekt();

    /// Serialisierung via cereal.
    template <class Archive> void serialize(Archive& ar) {
        ar(id, team, pos.x, pos.y, pos.z, orientation.x, orientation.y, orientation.z, orientation.w);
    }

protected:

    /// Einmalige ID.
    uint32_t id;

    /// Teamzugehörigkeit. 0 = Kein Team.
    uint8_t team = 0;

    /// Position.
    Ogre::Vector3 pos;

    /// Ausrichtung.
    Ogre::Quaternion orientation;

private:

    /// Vergebene IDs.
    static inline std::unordered_set<uint32_t> given_ids;

};
