#pragma once

#include "standard_ui.hpp"

class Nav_UI final : public Standard_UI {

public:

    void update_and_show(const Sub* sub) override;

};
