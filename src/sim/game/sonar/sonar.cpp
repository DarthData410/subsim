#include "sonar.hpp"
#include "../../physik.hpp"

Sonar::Sonar(float noise_threshold, float resolution, std::vector<std::tuple<float, float>> blindspots) :
    timer(0), noise(noise_threshold), resolution(resolution), blindspots(std::move(blindspots))
{
    //
}

bool Sonar::is_in_toter_winkel(winkel_t kurs_relativ) const {
    return std::any_of(blindspots.begin(), blindspots.end(), [&](const auto& blindspot) {
        return Physik::is_winkel_zwischen(kurs_relativ, std::get<0>(blindspot), std::get<0>(blindspot));
    });
}
