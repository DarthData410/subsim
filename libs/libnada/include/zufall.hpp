#pragma once

#include "log.hpp"
#include <random>
#include <chrono>
#include <unordered_map>

/// Methoden zum Liefern von Zufallszahlen.
namespace Zufall {

    /// Liefert eine Zufallszahl (ganzzahlig) im Bereich a <= N <= b (Template-Version).
    template<typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
    T get(T a, T b);

    /// Liefert eine Zufallszahl (Gleitkomma) im Bereich a <= N <= b (Template-Version).
    template<typename T, std::enable_if_t<std::is_floating_point<T>::value, int> = 0>
    T get(T a, T b);

    /// Liefert eine Zufallszahl im Bereich a <= N <= b (unsigned int).
    int i(int a, int b);

    /// Liefert eine Zufallszahl im Bereich a <= N <= b (long long int).
    long long ll(long long a, long long b);

    /// Liefert eine Zufallszahl im Bereich a <= N <= b (unsigned int).
    unsigned int ui(unsigned int a, unsigned int b);

    /// Liefert eine Zufallszahl im Bereich a <= N <= b (unsigned long).
    unsigned long ul(unsigned long a, unsigned long b);

    /// Liefert eine Zufallszahl im Bereich a <= N < b (float).
    float f(float a, float b);

    /// Liefert eine Zufallszahl im Bereich a <= N < b (double).
    double d(double a, double b);

    /// Gibt mit einer Wahrscheinlichkeit von `chance`% `true` zurück.
    bool b(unsigned int chance);

    /**
     * Liefert eine Zufallszahl im größtmöglichen Zahlenbereich. Z.B. für zufälliges Hashing.
     * @tparam T Datentyp des Zufallshashes, z.B. uint_32t.
     * @return
     */
    template <typename T> T random_hash() {
        thread_local std::random_device rd;
        thread_local std::mt19937 mt(rd());
        std::uniform_int_distribution<T> uid(0);
        return uid(mt);
    }

    /**
     * @brief Liefert ein Zufälliges Element gegebener Liste. Const-Version.
     * @tparam T Datentyp der Elemente in der Liste.
     * @param liste Nicht-leere Liste mit Elementen, aus denen zufällig eines ausgewählt werden soll.
     * @warning Liste darf nicht leer sein!
     */
    template <typename T>
    const typename T::value_type& wahl(const T& liste) {
        std::size_t n = Zufall::get<std::size_t>(0, liste.size() - 1);
        auto iter = std::cbegin(liste);
        std::advance(iter, n);
        return *iter;
    }

    /**
     * @brief Liefert ein Zufälliges Element gegebener Liste. Nicht-const Version.
     * @tparam T Datentyp der Elemente in der Liste.
     * @param liste Nicht-leere Liste mit Elementen, aus denen zufällig eines ausgewählt werden soll.
     * @warning Liste darf nicht leer sein!
     */
    template <typename T>
    typename T::value_type& wahl(T& liste) {
        std::size_t n = Zufall::get<std::size_t>(0, liste.size() - 1);
        auto iter = std::begin(liste);
        std::advance(iter, n);
        return *iter;
    }

    /**
     * @brief Liefert ein Zufälliges Element gegebener Liste und entfernt dieses.
     * @note Liste darf nicht leer sein.
     * @note Objekt wird via Zuweisung kopiert und zurückgegeben.
     */
    template <typename T>
    typename T::value_type wahl_erase(T& liste) {
        std::size_t n = get<std::size_t>(0, liste.size() - 1);
        auto iter = std::begin(liste);
        std::advance(iter, n);
        typename T::value_type objekt = *iter; // Kopie
        liste.erase(iter);
        return objekt;
    }

    /**
     * Liefert einen Vektor aus Objekten.
     * Jedes Objekt kommt "paar<A,B>.second" mal vor.
     * Beispielsweise zum Erstellen von Loslisten.
     * @tparam A Key-Typ der Map und gleichzeitig value_type des zurückgegebenen Vektors.
     * @tparam B Datentyp, der die Anzahl der Objekte B bestimmt, die in den Rückgabevektor geschrieben werden.
     * @param gewichtungen Zu verarbeitende Map von Gewichtungen.
     * @return Erzeugte Liste von A.
     */
    template <typename A, typename B>
    constexpr std::vector<A> get_gewichtungen(const std::unordered_map<A, B>& gewichtungen) {
        std::vector<A> v;
        v.reserve(gewichtungen.size() * 2);
        for (const auto& paar : gewichtungen) for (unsigned int i = 0; i < paar.second; ++i) v.push_back(paar.first);
        return v;
    }

}
