#pragma once

#include <vector>
#include <array>

// Prädeklarationen.
class Sub;

/// UI zur Anzeige vom Passiven Sonar.
class Sonar_UI final {

public:

    static constexpr unsigned HISTORY_SIZE = 40;

    Sonar_UI() = default;

    explicit Sonar_UI(const Sub* sub);



private:

    std::vector<std::array<std::vector<float>, HISTORY_SIZE>> sonar_data;

};
