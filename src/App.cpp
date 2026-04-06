#include <string>
#include "App.hpp"
#include "imgui.h"
#include "ControlComponent.hpp"

App::App() {
    m_components["Red"]   = std::make_unique<AmountSliderComponent>("Red");
    m_components["Green"] = std::make_unique<AmountSliderComponent>("Green");
    m_components["Blue"]  = std::make_unique<AmountSliderComponent>("Blue");

    m_activeKey = "Red";
}

void App::Run() {
    // Control Window
    ImGui::Begin("Control Panel");

    ImGui::Text("Select Active Slider:");
    for (auto const& [name, component] : m_components) {
        if (ImGui::Selectable(name.c_str(), m_activeKey == name)) {
            m_activeKey = name;
        }
    }

    ImGui::Separator();
    
    if (m_components.count(m_activeKey)) {
        m_components[m_activeKey]->DrawControlPanel();
    }
    
    // Capture the current state of our components
    ImVec4 mixColor = ImVec4(
        m_components["Red"]->GetAmount(),
        m_components["Green"]->GetAmount(),
        m_components["Blue"]->GetAmount(),
        1.0f
    );
    ImGui::End();

    // Visualization Window
    ImGui::Begin("Image Panel");
    // We use ColorButton as a quick way to show our mixed RGB result
    ImGui::ColorButton("Result", mixColor, ImGuiColorEditFlags_NoPicker, ImVec2(200, 200));
    ImGui::End();
}