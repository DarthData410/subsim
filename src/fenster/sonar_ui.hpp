#pragma once

#include <vector>
#include <array>
#include <OgreTimer.h>

// Prädeklarationen.
class Sub;

/// UI zur Anzeige vom Passiven Sonar.
class Sonar_UI final {

public:

    static constexpr unsigned HISTORY_SIZE = 40;

    Sonar_UI() = default;

    explicit Sonar_UI(const Sub* sub);

    void update_and_show(const Sub* sub);

private:

    void reset_sonar_data(const Sub* sub);

    void show(const std::array<std::vector<float>, HISTORY_SIZE>& histogram) const;

    std::vector<std::array<std::vector<float>, HISTORY_SIZE>> sonar_data;

    /// Updateintervall (Echtzeit) in ms.
    std::vector<unsigned> intervalle;
    std::vector<Ogre::Timer> timers;

};
