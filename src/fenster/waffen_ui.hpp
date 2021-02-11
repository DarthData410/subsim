#pragma once

#include "standard_ui.hpp"

class Waffen_UI final : public Standard_UI {

public:

    Waffen_UI() = default;

    explicit Waffen_UI(const Sub* sub);

    void update_and_show(const Sub* sub) override;

};