#include <string>
#include "App.hpp"
#include "imgui.h"
#include "ControlComponent.hpp"

void App::Run() {
    // Control Window
    ImGui::Begin("Control Panel");
    
    m_red.DrawControlPanel();
    m_green.DrawControlPanel();
    m_blue.DrawControlPanel();
    
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