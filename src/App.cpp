#include <GL/glew.h>
#include <string>
#include "App.hpp"
#include "imgui.h"
#include "Component.hpp"
#include "fractals/GPUMandelbrot.hpp"
#include "fractals/GPUJulia.hpp"
#include "fractals/GPULyapunov.hpp"
#include "fractals/GPUNewton.hpp"
#include  "fractals/GPUApollonianGasket.hpp"

App::App() {
    RegisterFractal<GPUMandelbrot>("Mandelbrot");
    RegisterFractal<GPUJulia>("Julia");
    RegisterFractal<GPULyapunov>("Lyapunov");
    RegisterFractal<GPUNewton>("Newton");
    RegisterFractal<GPUApollonianGasket>("Apollonian Gasket");
    
    m_activeKey = "Mandelbrot";
}

template <typename T>
void App::RegisterFractal(const std::string& name) {
    m_components[name] = std::make_unique<T>(name);
}

void App::Run() {
    // Control Window
    ImGui::Begin("Control Panel");

    ImGui::Text("Current Fractal:");
    if (ImGui::BeginCombo("##FractalSelector", m_components[m_activeKey]->GetLabel().c_str())) {
        
        for (auto const& [name, component] : m_components) {
            const bool isSelected = (m_activeKey == name);
            if (ImGui::Selectable(component->GetLabel().c_str(), isSelected)) {
                m_activeKey = name;
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        // 5. Always end a combo if BeginCombo returned true
        ImGui::EndCombo();
    }

    ImGui::Separator();
    
    auto it = m_components.find(m_activeKey);

    if (it != m_components.end()) {
        const auto& component = it->second;
        component->DrawControlPanel();
        component->DrawViewportInfo();
    }
    
    ImGui::End();

    ImGui::Begin("Fractal");
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

        ImTextureID tex = comp->GetResultTexture();
        ImGui::Image(tex, viewportSize);
    }
    ImGui::End();
}