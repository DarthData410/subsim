#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

/// Benutzung z.B. so: `if (static Do_Once _; _)`
struct Do_Once {
    explicit operator bool() { return std::exchange(b, false); }
    private: bool b = true;
};

/// Helfermethoden, die sonst in keine Kategorie passen.
namespace Etc {

    /// Löscht aus gegebenem String `str` alle chars `c`.
    void remove(std::string& str, const char c);

    /// Liest eine Datei mit gegebenem Pfad ein und fügt jede Zeile in einen Container via `push_back` hinzu.
    void zeilen_lesen(const std::string& pfad, std::vector<std::string>& liste, const uint8_t min_laenge = 2);

    /// Sowas wie `System.currentTimeMillis()` aus Java.
    unsigned long long millis();

    /**
     * Liefert einen std::vector aus std::string, die aus einem Text durch einen Separator geteilt wurden.
     * @param text Aufzuteilender Text
     * @param token char, bei dem aufgeteilt wird
     * @return Textteile
     */
    template<typename T = std::string>
    std::vector<T> tokenize(const T& text, const char token);

    /**
     * Liefert aus einer INI Datei (mit = als Trennzeichen) den Wert zu gegebenem Schlüssel (key).
     * @note Liefert einen leeren String, bei Lesefehlern.
     * @param datei Pfad zur INI-Datei.
     * @param key Auszulesender Schlüssel.
     * @return Wert des Schlüssels.
     */
    std::string get_aus_ini(const std::string& datei, const std::string& key);

    /**
     * Liefert aus einer INI Datei (mit = als Trennzeichen) die durch `token` getrennte Werte
     * zu gegebenem Schlüssel (key) als `std::vector<std::string>`.
     *
     * @param datei Pfad zur INI-Datei.
     * @param key Schlüssel in der INI, aus der die Werte gelesen werden.
     * @param token Trennzeichen der Werteliste.
     * @param append Ist `append` gegeben, wird dieser Wert allen Strings angehängt.
     * @return Liste der ausgelesenen Werte als String.
     */
    std::vector<std::string> vektor_aus_ini(const std::string& datei, const std::string& key, const char token, const std::string& append = "");

    /**
     * Liefert einen Wert aus der Konfigurationsdatei.
     * @param key Schlüssel für den herauszuparsenden Wert.
     * @return Wert aus der Konfigurationsdatei.
     */
    template <typename T>
    T get_cfg(const std::string& key, const T& standard = T{}) {
        try {
            const auto& s = get_aus_ini("config.ini", key);
            std::stringstream ss(s);
            T t;
            ss >> t;
            return t;
        } catch (std::exception& e) {
            std::cerr << key << " konnte nicht gelesen werden " << e.what() << '\n';
            return standard;
        }
    }

    /// Liefert eine Farbe mit Grünstich bei positivem Verhältnis > 1, Rotstich bei < 1.
    uint32_t get_farbe_nach_verhaeltnis(unsigned zaehler, unsigned nenner);

    /**
     * Liefert alle Dateipfade (relativ) aus gegebenem Ordner (relativ) mit gegebener Dateiendung (Case-Sensitiv).
     * @param endung z.B. "png", "jpg", "dat" usw. (ohne '.' davor)
     * @note Dateiendung Case-sensitiv.
     */
    std::vector<std::string> alle_dateien(const std::string& ordner, std::string endung);

    /// Entfernt aus gegebenen String alle Leerzeichen.
    void remove_whitespace(std::string& s);

    /// Vergleicht via < die Objekte, die sich hinter ihren Zeigern verbergen.
    template <typename T>
    bool ptr_compare(const T* o1, const T* o2) { return *o1 < *o2; }

}
