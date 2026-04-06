#include <string>
#include "App.hpp"
#include "imgui.h"
#include "ControlComponent.hpp"

App::App() {
    m_components[Channel::Red]   = std::make_unique<AmountSliderComponent>("Red");
    m_components[Channel::Green] = std::make_unique<AmountSliderComponent>("Green");
    m_components[Channel::Blue]  = std::make_unique<AmountSliderComponent>("Blue");

    m_activeKey = Channel::Red;
}

void App::Run() {
    // Control Window
    ImGui::Begin("Control Panel");

    ImGui::Text("Select Active Slider:");
    for (auto const& [name, component] : m_components) {
        if (ImGui::Selectable(component->GetLabel().c_str(), m_activeKey == name)) {
            m_activeKey = name;
        }
    }

    ImGui::Separator();
    
    if (m_components.count(m_activeKey)) {
        m_components[m_activeKey]->DrawControlPanel();
    }
    
    // Capture the current state of our components
    ImVec4 mixColor = ImVec4(
        m_components[Channel::Red]->GetAmount(),
        m_components[Channel::Green]->GetAmount(),
        m_components[Channel::Blue]->GetAmount(),
        1.0f
    );
    ImGui::End();

    // Visualization Window
    ImGui::Begin("Image Panel");
    // We use ColorButton as a quick way to show our mixed RGB result
    ImGui::ColorButton("Result", mixColor, ImGuiColorEditFlags_NoPicker, ImVec2(200, 200));
    ImGui::End();
}