#pragma once

#include <imgui.h>
#include <optional>

/// Einige Erweiterungen zu ImGui.
namespace ImGui::Nada {

    /**
     * Modifiziert vom Original: https://github.com/Flix01/imgui
     * @param label
     * @param p_value Zu setzender Wert
     * @param v_min Wert - Minimum
     * @param v_max Wert - Maximum
     * @param v_step Wert - Schrittweite
     * @param radius Knopfradius
     * @param thickness Dicke des Zeigers
     * @param fmt Tooltip Wertanzeige
     * @package value2 Optionaler zweiter Wert, der auf dem Knopf angezeigt werden kann
     * @return
     */
    bool KnobDegree(const char* label, float* p_value,
                    float v_min = 0.f, float v_max = 360.f, float v_step = 1.f,
                    float radius = 40.f, float thickness = 2.0f, const char* fmt = "%.0f°", const std::optional<float>& value2 = std::nullopt);

    /**
     * Zeigt am vorigen Element einen Tooltip,
     * wenn die Maus sich über diesem befindet.
     */
    void Tooltip(const char* text);

}
