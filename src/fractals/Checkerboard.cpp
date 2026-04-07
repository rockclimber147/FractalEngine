#include "fractals/CheckerboardComponent.hpp"
#include <cmath>

CheckerboardComponent::CheckerboardComponent(std::string name) 
    : m_name(std::move(name)), m_imguiID("##" + m_name) 
{
    // FractalComponent constructor handles glGenTextures
    UpdateTexture();
}

std::string CheckerboardComponent::GetLabel() const {
    return m_name;
}

void CheckerboardComponent::DrawControlPanel() {
    ImGui::Text("Checkerboard Configuration");
    
    // If the slider moves, we trigger the UpdateTexture hook
    if (ImGui::SliderFloat("Brightness", &m_brightness, 0.0f, 1.0f)) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Debug Info:");
    ImGui::Text("Pos: %.3f, %.3f", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.2f", m_zoom);
    ImGui::Text("m_height and width: %d %d", m_height, m_width);
}

void CheckerboardComponent::UpdateTexture() {
    if (m_pixelBuffer.empty() || m_width <= 0 || m_height <= 0) return;

    double aspect = (double)m_width / (double)m_height;
    unsigned char colorVal = static_cast<unsigned char>(m_brightness * 255.0f);

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            // Transform pixel (x,y) to World Space (u,v)
            // We center the view by subtracting 0.5 from the normalized coordinates
            double u = m_offsetX + ((x / (double)m_width) - 0.5) * (4.0 / m_zoom) * aspect;
            double v = m_offsetY + ((y / (double)m_height) - 0.5) * (4.0 / m_zoom);

            // Checkerboard math: flip color based on floor of world coordinates
            bool isVisible = (static_cast<int>(std::floor(u)) + static_cast<int>(std::floor(v))) % 2 == 0;
            
            int pixelIdx = (y * m_width + x) * 3;
            unsigned char finalColor = isVisible ? colorVal : 0;

            m_pixelBuffer[pixelIdx + 0] = finalColor; // R
            m_pixelBuffer[pixelIdx + 1] = finalColor; // G
            m_pixelBuffer[pixelIdx + 2] = finalColor; // B
        }
    }

    // Call the protected helper in FractalComponent to push pixels to GPU
    UploadTexture();
}