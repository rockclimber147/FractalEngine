#include "fractals/Mandelbrot/CPUMandelbrot.hpp"
#include <complex>
#include <cmath>
#include <iostream>

Mandelbrot::Mandelbrot(std::string name) 
    : m_name(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

std::string Mandelbrot::GetLabel() const { return m_name; }

void Mandelbrot::DrawControlPanel() {
    ImGui::Text("Mandelbrot");
    
    // Increased max iterations for better detailed edges
    if (ImGui::SliderInt("Detail (Iterations)", &m_maxIterations, 50, 2000)) {
        UpdateTexture();
    }

    // New Slider: Controls color density/cycling speed
    if (ImGui::SliderFloat("Color Cycle Speed", &m_colorFrequency, 1.0f, 100.0f)) {
        UpdateTexture();
    }

    if (ImGui::SliderFloat("Color Offset", &m_colorOffset, 0.0f, 6.0f)) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

void Mandelbrot::GetPixelColor(double u, double v, unsigned char rgb[3]) {
    double z_real = 0.0, z_imag = 0.0;
    double z_real2 = 0.0, z_imag2 = 0.0;
    int iter = 0;

    while (z_real2 + z_imag2 <= 4.0 && iter < m_maxIterations) {
        z_imag = 2.0 * z_real * z_imag + v;
        z_real = z_real2 - z_imag2 + u;
        z_real2 = z_real * z_real;
        z_imag2 = z_imag * z_imag;
        iter++;
    }

    if (iter < m_maxIterations) {
        // Smooth coloring logic...
        double nu = std::log(std::log(z_real2 + z_imag2) / 2.0 / std::log(2.0)) / std::log(2.0);
        MapColor((double)iter + 1.0 - nu, rgb);
    } else {
        rgb[0] = rgb[1] = rgb[2] = 0;
    }
}

void Mandelbrot::MapColor(double escapeVal, unsigned char* rgbOut) {
    double scale = m_colorFrequency / 100.0;
    
    double r = std::sin(scale * escapeVal + 0.0 + m_colorOffset) * 0.5 + 0.5; // (0 to 1 range)
    double g = std::sin(scale * escapeVal + 2.0 + m_colorOffset) * 0.5 + 0.5; 
    double b = std::sin(scale * escapeVal + 4.0 + m_colorOffset) * 0.5 + 0.5;

    // Convert to 0-255 range
    rgbOut[0] = static_cast<unsigned char>(r * 255);
    rgbOut[1] = static_cast<unsigned char>(g * 255);
    rgbOut[2] = static_cast<unsigned char>(b * 255);
}

double Mandelbrot::GetPixelsPerUnit() const { return 200.0 * m_zoom; }