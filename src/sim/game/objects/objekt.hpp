#include <Ogre.h>

class Welt;

/// Elternklasse für alle von der Physiksimulation betroffenen Objekte.
class Objekt {

public:

    enum class Typ {
        OBJEKT, OBJEKT_STEUERBAR, SUB, TORPEDO
    };

    Objekt() = default;

    explicit Objekt(const Ogre::Vector3& pos, const Ogre::Quaternion& orientation =
                    Ogre::Quaternion::IDENTITY); //Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_Y));

    virtual Typ get_typ() const = 0;

    virtual void tick(Welt* welt, float s) = 0;

    uint32_t get_id() const { return id; }

    const Ogre::Vector3& get_pos() const { return pos; }

    const Ogre::Quaternion& get_orientation() const { return orientation; }

    float get_bearing() const;

    float get_pitch() const;

    virtual ~Objekt();

protected:

    /// Einmalige ID.
    uint32_t id;

    /// Position.
    Ogre::Vector3 pos;

    /// Ausrichtung.
    Ogre::Quaternion orientation;

private:

    /// Vergebene IDs.
    static inline std::unordered_set<uint32_t> given_ids;

};
