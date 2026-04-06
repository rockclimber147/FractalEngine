#include <string>
#include "App.hpp"
#include "imgui.h"
#include "ControlComponent.hpp"

void App::Run() {
    // Control Window
    ImGui::Begin("Control Panel");

    ImGui::Text("Select Active Slider:");
    if (ImGui::RadioButton("Red", (int*)&m_activeChannel, (int)ColorChannel::Red)) {} 
    ImGui::SameLine();
    if (ImGui::RadioButton("Green", (int*)&m_activeChannel, (int)ColorChannel::Green)) {} 
    ImGui::SameLine();
    if (ImGui::RadioButton("Blue", (int*)&m_activeChannel, (int)ColorChannel::Blue)) {}

    ImGui::Separator();
    
    switch (m_activeChannel) {
        case ColorChannel::Red:
            m_red.DrawControlPanel();
            break;
        case ColorChannel::Green:
            m_green.DrawControlPanel();
            break;
        case ColorChannel::Blue:
            m_blue.DrawControlPanel();
            break;
    }
    
    // Capture the current state of our components
    ImVec4 mixColor = ImVec4(
        m_red.GetAmount(), 
        m_green.GetAmount(), 
        m_blue.GetAmount(), 
        1.0f
    );
    
    ImGui::End();

    // Visualization Window
    ImGui::Begin("Image Panel");
    // We use ColorButton as a quick way to show our mixed RGB result
    ImGui::ColorButton("Result", mixColor, ImGuiColorEditFlags_NoPicker, ImVec2(200, 200));
    ImGui::End();
}