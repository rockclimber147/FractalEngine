#include <string>
#include "App.hpp"
#include "imgui.h"
#include "Component.hpp"
#include "fractals/CheckerboardComponent.hpp"

App::App() {
    m_components["Checkerboard"] = std::make_unique<CheckerboardComponent>("Checkerboard");
    
    // Set the default active component
    m_activeKey = "Checkerboard";
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
    
    ImGui::End();

    ImGui::Begin("Pixel Panel");
    if (m_components.count(m_activeKey)) {
        auto& comp = m_components[m_activeKey];
        
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        
        comp->Resize((int)viewportSize.x, (int)viewportSize.y);

        if (ImGui::IsWindowHovered() && ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            comp->Pan(delta.x, delta.y, viewportSize.x, viewportSize.y);
        }

        if (ImGui::IsWindowHovered() && ImGui::GetIO().MouseWheel != 0) {
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 cursorPadding = ImGui::GetCursorPos();

            float relativeX = mousePos.x - windowPos.x - cursorPadding.x;
            float relativeY = mousePos.y - windowPos.y - cursorPadding.y;

            comp->Zoom(ImGui::GetIO().MouseWheel, relativeX, relativeY, viewportSize.x, viewportSize.y);
        }

        // 5. Render the result
        ImTextureID tex = comp->GetResultTexture();
        ImGui::Image(tex, viewportSize);
    }
    ImGui::End();
}