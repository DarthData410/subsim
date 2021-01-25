#include "team.hpp"

Team::Team() {
    static uint8_t id_counter = 1;
    id = id_counter++;
}
