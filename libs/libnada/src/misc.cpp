#include "nada/misc.hpp"
#include "nada/log.hpp"

#include <chrono>
#include <filesystem>
#include <unordered_map>

template std::vector<std::string> nada::misc::tokenize(const std::string& text, const char token);
template<typename T> std::vector<T> nada::misc::tokenize(const T& text, const char token) {
    std::vector<T> teile;
    unsigned long long anfang = 0;
    unsigned long long ende;
    while ((ende = text.find(token, anfang)) != T::npos) {
        teile.push_back(text.substr(anfang, ende - anfang));
        anfang = ende;
        anfang++;
    }
    teile.push_back(text.substr(anfang));
    return teile;
}

void nada::misc::remove(std::string& str, const char c) {
    str.erase(std::remove(std::begin(str), std::end(str), c), std::end(str));
}

void nada::misc::zeilen_lesen(const std::string& pfad, std::vector<std::string>& liste, uint8_t min_laenge) {
    std::ifstream in(pfad, std::ios::binary);
    if (in.good()) {
        for (std::string zeile; std::getline(in, zeile);) {
            if (zeile.size() >= min_laenge) {
                nada::misc::remove(zeile, '\r'); // Windows workaround
                liste.push_back(zeile);
            }
        }
        if (liste.empty()) nada::Log::err() << "Keine Elemente in " << pfad << '\n';
    }
    else nada::Log::err() << pfad << " konnte nicht gelesen werden.\n";
}

std::string nada::misc::get_aus_ini(const std::string& datei, const std::string& key) {
    const char token = '=';
    thread_local std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ini_cache;
    if (!ini_cache.count(datei)) {
        ini_cache[datei];
        if (std::ifstream in(datei); in.good()) {
            for (std::string zeile; std::getline(in, zeile);) {
                if (zeile.find(token) != std::string::npos) {
                    const std::vector<std::string>& tokens = tokenize(zeile, token);
                    if (tokens.size() >= 2) ini_cache[datei][tokens[0]] = tokens[1];
                }
            }
        } 
        else Log::err() << "misc::" << __func__ <<  datei << " nicht lesbar.\n";
    }
    return ini_cache[datei][key];
}

std::vector<std::string>
nada::misc::vektor_aus_ini(const std::string& datei, const std::string& key, const char token, const std::string& append) {
    std::vector<std::string> tokens = nada::misc::tokenize(nada::misc::get_aus_ini(datei, key), token);
    if (!append.empty()) for (auto& t : tokens) t.append(append);
    return tokens;
}

unsigned long long nada::misc::millis() {
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return millis.count();
}

uint32_t nada::misc::get_farbe_nach_verhaeltnis(unsigned int zaehler, unsigned int nenner) {
    const float verhaeltnis = static_cast<float>(zaehler) / static_cast<float>(nenner);
    const uint8_t g = std::min(0xFF, static_cast<int>(0xFF * verhaeltnis));
    const uint8_t r = 0xFF - g;
    const uint8_t arr[4] = {r, g, 0x40, 0xFF};
    uint32_t f;
    uint8_t* p = reinterpret_cast<uint8_t*>(&f);
    std::move(&arr[0], &arr[4], p);
    return f;
}

std::vector<std::string> nada::misc::alle_dateien(const std::string& ordner, std::string endung) {
    std::vector<std::string> dateien;
    endung = "." + endung;
    for (const auto& entry : std::filesystem::directory_iterator(ordner)) {
        if (entry.is_regular_file()) if (const auto& dateiname = entry.path().string();
                dateiname.find(endung) != std::string::npos &&
                dateiname.substr(dateiname.find(endung)).size() == endung.size() + 1)
        {
            dateien.push_back(dateiname);
        }
    }
    return dateien;
}

void nada::misc::remove_whitespace(std::string& s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); }), s.end());
}
