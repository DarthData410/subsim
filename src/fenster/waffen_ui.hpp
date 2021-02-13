#pragma once

#include "standard_ui.hpp"

class Waffen_UI final : public Standard_UI {

public:

    Waffen_UI();

    explicit Waffen_UI(Klient* klient);

    void update_and_show(const Sub* sub) override;

};