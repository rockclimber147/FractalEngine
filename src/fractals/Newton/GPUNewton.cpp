#include "fractals/Newton/GPUNewton.hpp"

GPUNewton::GPUNewton(std::string name) 
    : GPUFractalComponent(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

void GPUNewton::UploadExtraUniforms() {
}

void GPUNewton::DrawControlPanel() {
    bool changed = false;

    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

