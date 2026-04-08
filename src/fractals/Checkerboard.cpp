#include <cmath>
#include "fractals/Checkerboard.hpp"

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

void Checkerboard::UpdateTexture() {
    if (m_pixelBuffer.empty()) return;

    double pixelsPerUnit = 100.0 * m_zoom;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            double u = m_offsetX + (x - m_width * 0.5) / pixelsPerUnit;
            double v = m_offsetY + (y - m_height * 0.5) / pixelsPerUnit;

            int checkX = static_cast<int>(std::floor(u));
            int checkY = static_cast<int>(std::floor(v));
            bool isVisible = (std::abs(checkX + checkY) % 2 == 0);
            
            int idx = (y * m_width + x) * 3;
            unsigned char color = isVisible ? (unsigned char)(m_brightness * 255) : 0;
            
            m_pixelBuffer[idx] = m_pixelBuffer[idx+1] = m_pixelBuffer[idx+2] = color;
        }
    }
    UploadTexture();
}