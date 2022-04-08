#include "sonar_aktiv.hpp"

Sonar_Aktiv::Sonar_Aktiv(float noise_threshold, float resolution,
                         const std::vector<std::tuple<float, float>>& blindspots, float ping_intervall_min) : Sonar(
        noise_threshold, resolution, blindspots), ping_intervall_min(ping_intervall_min)
{

}
