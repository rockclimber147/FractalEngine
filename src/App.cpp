#include "App.hpp"
#include "imgui.h"

App::App() : m_brightness(1.0f) {
}

void App::Run() {
    // 1. Control Panel
    ImGui::Begin("Control Panel");
    
    ImGui::Text("Adjust Image Settings");
    // Slider modifies our member variable directly
    ImGui::SliderFloat("Brightness", &m_brightness, 0.0f, 2.0f);
    
    if (ImGui::Button("Reset")) {
        m_brightness = 1.0f;
    }
    
    ImGui::End();

    // 2. Image Panel
    ImGui::Begin("Image Panel");

    // We use the shared m_brightness variable to affect the "image"
    // Here we use a colored dummy or a tint to simulate the effect
    ImVec4 tint = ImVec4(m_brightness, m_brightness, m_brightness, 1.0f);
    
    ImGui::Text("Preview:");
    // If you have a texture ID from your loader:
    // ImGui::Image((void*)(intptr_t)myTextureID, ImVec2(400, 300), ImVec2(0,0), ImVec2(1,1), tint);
    
    // For now, let's visualize it with a colored rectangle
    ImGui::ColorButton("ImagePlaceholder", tint, 0, ImVec2(300, 200));
    
    ImGui::Text("Current Intensity: %.2f", m_brightness);

    ImGui::End();
}