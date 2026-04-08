#include <cmath>
#include "fractals/Checkerboard/Checkerboard.hpp"

Checkerboard::Checkerboard(std::string name) 
    : m_name(std::move(name)), m_imguiID("##" + m_name) 
{
    // FractalComponent constructor handles glGenTextures
    UpdateTexture();
}

std::string Checkerboard::GetLabel() const {
    return m_name;
}

void Checkerboard::DrawControlPanel() {
    ImGui::Text("Checkerboard Configuration");
    
    if (ImGui::SliderFloat("Brightness", &m_brightness, 0.0f, 1.0f)) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Debug Info:");
    ImGui::Text("Pos: %.3f, %.3f", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.2f", m_zoom);
    ImGui::Text("m_height and width: %d %d", m_height, m_width);
}

void Checkerboard::GetPixelColor(double u, double v, unsigned char rgb[3]) {
    int checkX = static_cast<int>(std::floor(u));
    int checkY = static_cast<int>(std::floor(v));
    bool isVisible = (std::abs(checkX + checkY) % 2 == 0);
    
    unsigned char color = isVisible ? (unsigned char)(m_brightness * 255) : 0;
    rgb[0] = rgb[1] = rgb[2] = color;
}