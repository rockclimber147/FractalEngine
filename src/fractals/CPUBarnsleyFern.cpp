#include "fractals/CPUBarnsleyFern.hpp"

CPUBarnsleyFern::CPUBarnsleyFern(std::string name) : CPUFractalComponent(std::move(name))
{
    m_zoom = 0.5;
    UpdateTexture();
}

void CPUBarnsleyFern::GenerateFractalData() {
    double x = 0, y = 0;
    double ppu = GetPixelsPerUnit();
    
    // Run the "Chaos Game" for a few million iterations
    for (int i = 0; i < 2000000; i++) {
        double r = (double)rand() / RAND_MAX;
        double nextX, nextY;

        // Barnsley's 4 magic transformations
        if (r < 0.01) {
            nextX = 0;
            nextY = 0.16 * y;
        } else if (r < 0.86) {
            nextX = 0.85 * x + 0.04 * y;
            nextY = -0.04 * x + 0.85 * y + 1.6;
        } else if (r < 0.93) {
            nextX = 0.2 * x - 0.26 * y;
            nextY = 0.23 * x + 0.22 * y + 1.6;
        } else {
            nextX = -0.15 * x + 0.28 * y;
            nextY = 0.26 * x + 0.24 * y + 0.44;
        }
        x = nextX; y = nextY;

        // Convert world coordinates (x, y) to pixel coordinates (px, py)
        int px = (int)((x - m_offsetX) * ppu + m_width / 2);
        int py = (m_height - 1) - (int)((y - m_offsetY) * ppu + m_height / 2);

        // If the point is on screen, "paint" it green
        if (px >= 0 && px < m_width && py >= 0 && py < m_height) {
            int idx = (py * m_width + px) * 3;
            m_pixelBuffer[idx + 1] = 200; // Green channel
        }
    }
}

void CPUBarnsleyFern::DrawControlPanel() {

}