#include "fractals/core/CPUFractalComponent.hpp"

CPUFractalComponent::CPUFractalComponent(std::string name) : FractalComponent(std::move(name)) {
    InitTexture();
}

void CPUFractalComponent::Resize(int w, int h) {
    if (w == m_width && h == m_height) return;
    if (w <= 0 || h <= 0) return;
    m_width = w;
    m_height = h;
    m_pixelBuffer.assign(m_width * m_height * 3, 0); // CPU-only memory allocation
    UpdateTexture();
}

void CPUFractalComponent::UpdateTexture() {
    if (m_pixelBuffer.empty() || m_width <= 0 || m_height <= 0) return;

    double ppu = GetPixelsPerUnit();
    double halfW = m_width * 0.5;
    double halfH = m_height * 0.5;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            double u = m_offsetX + (x - halfW) / ppu;
            double v = m_offsetY + (y - halfH) / ppu;

            unsigned char rgb[3];
            GetPixelColor(u, v, rgb);

            int idx = (y * m_width + x) * 3;
            m_pixelBuffer[idx + 0] = rgb[0];
            m_pixelBuffer[idx + 1] = rgb[1];
            m_pixelBuffer[idx + 2] = rgb[2];
        }
    }
    UploadTexture();
}

void CPUFractalComponent::InitTexture() {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void CPUFractalComponent::UploadTexture() {
    if (m_textureID == 0 || m_pixelBuffer.empty()) return;
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixelBuffer.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}