#include "fractals/CPUBarnsleyFern.hpp"

CPUBarnsleyFern::CPUBarnsleyFern(std::string name) : CPUFractalComponent(std::move(name))
{
    m_zoom = 0.5;
    UpdateTexture();
}

void CPUBarnsleyFern::GenerateFractalData() {
    double x = 0, y = 0;
    double ppu = GetPixelsPerUnit();
    
    for (int i = 0; i < m_iterationCount; i++) {
        double r = (double)rand() / RAND_MAX;
        double nextX, nextY;

        if (r < 0.01) { // Stem
            nextX = 0;
            nextY = 0.16 * y;
        } else if (r < 0.86) { // Main Body
            nextX = m_mainScale * x + m_mainCurvature * y;
            nextY = -m_mainCurvature * x + m_mainScale * y + m_mainHeight;
        } else if (r < 0.93) { // Left Leaflet
            nextX = 0.2 * x - 0.26 * y;
            nextY = 0.23 * x + 0.22 * y + 1.6;
        } else { // Right Leaflet
            nextX = -0.15 * x + 0.28 * y;
            nextY = 0.26 * x + 0.24 * y + 0.44;
        }
        
        x = nextX; y = nextY;

        int px = (int)((x - m_offsetX) * ppu + m_width / 2);
        int py = (m_height - 1) - (int)((y - m_offsetY) * ppu + m_height / 2);

        if (px >= 0 && px < m_width && py >= 0 && py < m_height) {
            int idx = (py * m_width + px) * 3;
            m_pixelBuffer[idx + 1] = 200; 
        }
    }
}

void CPUBarnsleyFern::DrawControlPanel() {
    bool changed = false;
    
    ImGui::Text("Main Stem (T2)");
    if (ImGui::SliderFloat("Scale", &m_mainScale, 0.5f, 0.95f)) changed = true;
    if (ImGui::SliderFloat("Curvature", &m_mainCurvature, -0.2f, 0.2f)) changed = true;
    if (ImGui::SliderFloat("Internode", &m_mainHeight, 0.5f, 3.0f)) changed = true;

    ImGui::Separator();
    ImGui::Text("Leaflet Growth (T3/T4)");
    if (ImGui::SliderFloat("Side Scale", &m_sideScale, 0.1f, 0.5f)) changed = true;
    
    ImGui::Separator();
    if (ImGui::SliderInt("Points", &m_iterationCount, 100000, 5000000)) changed = true;

    if (changed) UpdateTexture();
}