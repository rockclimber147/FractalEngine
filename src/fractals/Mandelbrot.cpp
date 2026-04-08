#include "fractals/Mandelbrot.hpp"
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

void Mandelbrot::UpdateTexture() {
    if (m_pixelBuffer.empty() || m_width <= 0 || m_height <= 0) return;

    double pixelsPerUnit = 200.0 * m_zoom;

    // Standard Math constants needed for smooth coloring
    const double LOG2 = std::log(2.0);

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            // 1. Map pixel to complex plane
            double u = m_offsetX + (x - m_width * 0.5) / pixelsPerUnit;
            double v = m_offsetY + (y - m_height * 0.5) / pixelsPerUnit;

            // 2. Escape-time algorithm
            double z_real = 0.0;
            double z_imag = 0.0;
            int iter = 0;

            // Magnitude squared must remain <= 4
            double z_real2 = 0.0;
            double z_imag2 = 0.0;

            while (z_real2 + z_imag2 <= 4.0 && iter < m_maxIterations) {
                double next_real = z_real2 - z_imag2 + u;
                z_imag = 2.0 * z_real * z_imag + v;
                z_real = next_real;
                
                // Recalculate squares for next iteration and escape check
                z_real2 = z_real * z_real;
                z_imag2 = z_imag * z_imag;
                iter++;
            }

            // 3. Coloring Logic
            int idx = (y * m_width + x) * 3;
            unsigned char rgb[3];

            if (iter < m_maxIterations) {
                // Calculate Continuous Escape Time (Smooth Color)
                double magZ2 = z_real2 + z_imag2;
                double logMagZ = std::log(magZ2) * 0.5; // log(|z|) = log(|z|^2) / 2
                double nu = std::log(logMagZ / LOG2) / LOG2;
                
                // Final value is Smooth (continuous) iteration count
                double escapeVal = (double)iter + 1.0 - nu;
                
                // Map that smooth value to a color using our new parameter
                MapColor(escapeVal, rgb);
            } else {
                // Points inside the set are solid black
                rgb[0] = rgb[1] = rgb[2] = 0;
            }

            // Copy RGB into pixel buffer
            m_pixelBuffer[idx + 0] = rgb[0];
            m_pixelBuffer[idx + 1] = rgb[1];
            m_pixelBuffer[idx + 2] = rgb[2];
        }
    }
    UploadTexture();
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