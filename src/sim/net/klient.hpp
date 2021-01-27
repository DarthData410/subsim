#pragma once

#include <iostream>

class Klient final {

public:

    Klient() = default;

    explicit Klient(const std::string& ip);

    ~Klient();

    void test() {
        std::cout << "Klient: wuppi." << std::endl;

    }

private:



};