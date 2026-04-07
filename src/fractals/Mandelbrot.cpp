#include "fractals/MandelbrotComponent.hpp"
#include <complex>

MandelbrotComponent::MandelbrotComponent(std::string name) 
    : m_name(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

std::string MandelbrotComponent::GetLabel() const { return m_name; }

void MandelbrotComponent::DrawControlPanel() {
    ImGui::Text("Mandelbrot Set");
    
    // Iterations dictate the "detail" at the edges of the fractal
    if (ImGui::SliderInt("Max Iterations", &m_maxIterations, 10, 1000)) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Viewport: %.2f, %.2f", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

void MandelbrotComponent::UpdateTexture() {
    if (m_pixelBuffer.empty() || m_width <= 0 || m_height <= 0) return;

    // Use the same scale logic as checkerboard for consistency
    double pixelsPerUnit = 200.0 * m_zoom;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            // 1. Map pixel to complex plane (c = u + vi)
            double u = m_offsetX + (x - m_width * 0.5) / pixelsPerUnit;
            double v = m_offsetY + (y - m_height * 0.5) / pixelsPerUnit;

            // 2. Escape-time algorithm
            double z_real = 0.0;
            double z_imag = 0.0;
            int iter = 0;

            // Check if |z| > 2 (z_real^2 + z_imag^2 > 4)
            while (z_real * z_real + z_imag * z_imag <= 4.0 && iter < m_maxIterations) {
                // z = z^2 + c
                // (a+bi)^2 = a^2 - b^2 + 2abi
                double next_real = z_real * z_real - z_imag * z_imag + u;
                double next_imag = 2.0 * z_real * z_imag + v;
                z_real = next_real;
                z_imag = next_imag;
                iter++;
            }

            // 3. Simple Grayscale Coloring
            int idx = (y * m_width + x) * 3;
            unsigned char color = 0;
            
            if (iter < m_maxIterations) {
                // Map iteration count to a brightness value
                color = static_cast<unsigned char>(255 * (double)iter / m_maxIterations);
            }

            m_pixelBuffer[idx] = m_pixelBuffer[idx+1] = m_pixelBuffer[idx+2] = color;
        }
    }
    UploadTexture();
}