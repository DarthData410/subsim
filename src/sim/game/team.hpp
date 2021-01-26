#pragma once

#include "objects/sub.hpp"
#include <vector>

class Team final {

public:

    /// Vergibt eine einmalige ID ans Team.
    Team() = default;

    Team(uint8_t id);

    /// Gibt ein Design für ein neues Sub.
    const Sub& get_new_sub();

private:

    /// Einmalige ID.
    uint8_t id;

    /// Verfügbare Uboote des Teams.
    std::vector<Sub> sub_designs;

};
