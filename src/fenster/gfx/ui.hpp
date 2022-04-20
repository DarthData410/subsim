#pragma once

#include <optional>
#include <imgui.h>

namespace ui {


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
                    float radius = 40.f, float thickness = 2.0f,
                    const char* fmt = "%.0f°", const std::optional<float>& value2 = std::nullopt);

    /**
     * Zeigt am vorigen Element einen Tooltip,
     * wenn die Maus sich über diesem befindet.
     */
    void Tooltip(const char* text);

    /// Wrapper für ImGui::XXX
    void Text(const char* fmt, ...);
    void TextUnformatted(const char* text);
    void Separator();
    bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
    bool Checkbox(const char* label, bool* v);
    bool RadioButton(const char* label, bool active);
    bool RadioButton(const char* label, int* v, int v_button);
    bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
    bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* format = "%d", ImGuiSliderFlags flags = 0);
    bool InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);

}
