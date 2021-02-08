#include "sonar_ui.hpp"
#include "../sim/game/objects/sub.hpp"

Sonar_UI::Sonar_UI(const Sub* sub) {
    for (const auto& sonar : sub->get_sonars()) {
        sonar_data.emplace_back();
        for (auto& vektor : sonar_data.back()) vektor = std::vector<float>(sonar.get_resolution(), 0.f);
    }
}
