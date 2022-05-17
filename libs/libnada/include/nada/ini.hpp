#pragma once

#include "misc.hpp"

namespace nada {

    class Ini final {

        explicit Ini(const std::string& ini_path) : datei(ini_path) {}

        bool good() const { if (std::ifstream in(datei); in.good()) return true; return false; }

        std::string get(const std::string& key, const std::string& fallback = "") {
            const std::string val = misc::get_aus_ini(datei, key);
            if (val.empty()) return fallback;
            return val;
        }

        template<typename T>
        T get_int(const std::string& key, T fallback = T{}) {
            const std::string val = misc::get_aus_ini(datei, key);
            if (val.empty()) return fallback;
            return std::atoll(val.c_str());
        }

        template<typename T>
        T get_float(const std::string& key, T fallback = T{}) {
            const std::string val = misc::get_aus_ini(datei, key);
            if (val.empty()) return fallback;
            return std::atof(val.c_str());
        }

    private:

        std::string datei;

    };

}
