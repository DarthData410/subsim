#include "etc.hpp"
#include "log.hpp"
#include <chrono>
#include <filesystem>

template std::vector<std::string> Etc::tokenize(const std::string& text, const char token);

template<typename T>
std::vector<T> Etc::tokenize(const T& text, const char token) {
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

void Etc::remove(std::string& str, const char c) {
    str.erase(std::remove(std::begin(str), std::end(str), c), std::end(str));
}

void Etc::zeilen_lesen(const std::string& pfad, std::vector<std::string>& liste, uint8_t min_laenge) {
    std::ifstream in(pfad, std::ios::binary);
    if (in.good()) {
        for (std::string zeile; std::getline(in, zeile);) {
            if (zeile.size() >= min_laenge) {
                Etc::remove(zeile, '\r'); // Windows workaround
                liste.push_back(zeile);
            }
        }
        if (liste.empty()) Log::err() << "Keine Elemente in " << pfad << '\n';
    }
    else Log::err() << pfad << " konnte nicht gelesen werden.\n";
}

std::string Etc::get_aus_ini(const std::string& datei, const std::string& key) {
    if (key.empty()) return "";
    const char token = '=';
    if (std::ifstream in(datei); in.good()) {
        for (std::string zeile; std::getline(in, zeile);) {
            if (zeile.empty()) continue;
            if (zeile[0] != key[0]) continue;
            if (zeile.find(token) != std::string::npos) {
                const std::vector<std::string>& tokens = tokenize(zeile, token);
                if (tokens[0].find(key) == std::string::npos) continue;
                std::string wert = tokens[1];
                if (!wert.empty()) {
                    wert.erase(remove_if(wert.begin(), wert.end(), ::isspace), wert.end());
                    return wert;
                }
            }
        }
        Log::err() << "Etc::" << __func__ << " key " << key << " in " << datei << " nicht gefunden.\n";
    }
    else Log::err() << "Etc::" << __func__ <<  datei << " nicht lesbar.\n";
    return {""};
}

std::vector<std::string>
Etc::vektor_aus_ini(const std::string& datei, const std::string& key, const char token, const std::string& append) {
    std::vector<std::string> tokens = Etc::tokenize(Etc::get_aus_ini(datei, key), token);
    if (!append.empty()) for (auto& t : tokens) t.append(append);
    return tokens;
}

unsigned long long Etc::millis() {
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    return millis.count();
}

uint32_t Etc::get_farbe_nach_verhaeltnis(unsigned int zaehler, unsigned int nenner) {
    const float verhaeltnis = static_cast<float>(zaehler) / static_cast<float>(nenner);
    const uint8_t g = std::min(0xFF, static_cast<int>(0x80 * verhaeltnis));
    const uint8_t r = 0xFF - g;
    const uint8_t arr[4] = {r, g, 0x40, 0xFF};
    uint32_t f;
    uint8_t* p = reinterpret_cast<uint8_t*>(&f);
    std::move(&arr[0], &arr[4], p);
    return f;
}

std::vector<std::string> Etc::alle_dateien(const std::string& ordner, std::string endung) {
    std::vector<std::string> dateien;
    endung = "." + endung;
    for (const auto& entry : std::filesystem::directory_iterator(ordner)) {
        if (entry.is_regular_file()) if (const auto& dateiname = entry.path().string();
                dateiname.find(endung) != std::string::npos &&
                dateiname.substr(dateiname.find(endung)).size() == 4)
        {
            dateien.push_back(dateiname);
        }
    }
    return dateien;
}

void Etc::remove_whitespace(std::string& s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](unsigned char c) { return std::isspace(c); }), s.end());
}
