#include "nav_ui.hpp"
#include "../sim/net/klient.hpp"
#include "../sim/physik.hpp"
#include "gfx/ui.hpp"

#include <implot.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

Nav_UI::Nav_UI() : Standard_UI(nullptr) {}

Nav_UI::Nav_UI(Klient* klient) : Standard_UI(klient) {}

void Nav_UI::update_and_show(const Sub* sub) {
    ui::Font f(ui::FONT::MONO_16);
    show_navigation(sub);
    show_noise_signature(sub, sub->get_speed());
}

void Nav_UI::show_navigation(const Sub* sub) const {
    ImGui::SetNextWindowSize({300,0});
    ImGui::Begin("Nav View");
    ui::Text("Sub: %.1f %.1f Depth: %.1f", sub->get_pos().x(), sub->get_pos().y(), sub->get_pos().z());
    ui::Text("Pitch:   %.1f", sub->get_pitch());
    ui::Text("Bearing: %.1f (Target: %.f)", sub->get_bearing(), sub->get_target_bearing());
    ui::Text("Speed:   %.1f (Target: %.f)", sub->get_speed(), sub->get_target_speed());

/*    static float target_x = 0, target_z = 0; // - ohne auto-pos mehr immersion?
    ImGui::InputFloat("Target_x", &target_x);
    ImGui::InputFloat("Target_z", &target_z);
    if (ImGui::Button("Set##set_target_pos")) {
        klient->kommando({Kommando::AUTO_POS, sub->get_id(),
                          std::tuple<float, float>(target_x, target_z)});
    }*/

    static float target_bearing = 0;
    ui::KnobDegree("target_bearing", &target_bearing,
                            0.f, 360.f, 1.f, 40.f, 2.f, "%.0f°", std::make_optional(sub->get_bearing()));
    if (ui::Button("Set##set_bearing")) {
        klient->kommando({Kommando::AUTO_KURS, sub->get_id(), target_bearing});
    }
    ui::Separator();

    static float target_speed = 0;
    ui::SliderFloat("Set Speed", &target_speed, -sub->get_speed_max(), sub->get_speed_max(), "%.1f");
    ui::MouseWheel(target_speed, 0.5f, -sub->get_speed_max(), sub->get_speed_max());
    if (ui::Button("Set##set_speed")) {
        klient->kommando({Kommando::MOTOR_LINEAR, sub->get_id(), target_speed / sub->get_speed_max()});
    }
    ui::Separator();

    if (ui::Button("stop")) {
        klient->kommando({Kommando::STOP, sub->get_id()});
    }
    if (ui::Button("rechts")) {
        klient->kommando({Kommando::MOTOR_ROT, sub->get_id(), 100.f});
    }
    if (ui::Button("links")) {
        klient->kommando({Kommando::MOTOR_ROT, sub->get_id(), -100.f});
    }
    ImGui::End();
}

void Nav_UI::show_noise_signature(const Sub* sub, std::optional<float> mark_v) const {
    ImGui::Begin("Noise signature");
    const auto x_max = sub->get_speed_max() + 1;
    ImPlot::SetNextAxisLimits(ImAxis_X1, 0, x_max);
    ImPlot::SetNextAxisLimits(ImAxis_Y1, 0, 1);
    if (ImPlot::BeginPlot("Noise Level", {-1,0}, ImPlotFlags_NoMenus | ImPlotFlags_Crosshairs)) {
        ImPlot::SetupAxes("v in m/s", "Noise Level", ImPlotAxisFlags_Lock, ImPlotAxisFlags_Lock);
        std::vector<float> data_x;
        std::vector<float> data_y_1;
        std::vector<float> data_y_10;
        std::vector<float> data_y_25;
        std::vector<float> data_y_50; // TODO cache those by sub
        for (int v = 0; v <= x_max; v++) {
            data_x.push_back(v);
            data_y_1.push_back(Physik::sichtbarkeit(sub->get_noise(),  static_cast<float>(v), 1000));
            data_y_10.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 10000));
            data_y_25.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 25000));
            data_y_50.push_back(Physik::sichtbarkeit(sub->get_noise(), static_cast<float>(v), 50000));
        }
        if (mark_v) ImPlot::TagX(mark_v.value(), ImColor(0xFF, 0xFF, 0xFF));
        ImPlot::PlotLine("@ 1km", data_x.data(), data_y_1.data(), data_x.size());
        ImPlot::PlotLine("@ 10km", data_x.data(), data_y_10.data(), data_x.size());
        ImPlot::PlotLine("@ 25km", data_x.data(), data_y_25.data(), data_x.size());
        ImPlot::PlotLine("@ 50km", data_x.data(), data_y_50.data(), data_x.size());
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void Nav_UI::draw_gfx(const Sub* sub, sf::RenderWindow* window) {
    (void)sub;
    (void)window;
}
