#include "FractalComponent.hpp"

FractalComponent::FractalComponent() {
    InitTexture();
}

FractalComponent::~FractalComponent() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

void FractalComponent::Pan(float dx, float dy, float vW, float vH) {
    double aspect = (double)vW / vH;
    m_offsetX -= (dx / vW) * (4.0 / m_zoom) * aspect;
    m_offsetY -= (dy / vH) * (4.0 / m_zoom);
    UpdateTexture();
}

void FractalComponent::Zoom(float amount) {
    // Scroll up (positive) zooms in
    if (amount > 0) m_zoom *= 1.1;
    else if (amount < 0) m_zoom /= 1.1;
    UpdateTexture();
}

void FractalComponent::Resize(int w, int h) {
    if (w == m_width && h == m_height) return;
    m_width = w;
    m_height = h;
    m_pixelBuffer.assign(m_width * m_height * 3, 0);
    UpdateTexture();
}

void FractalComponent::InitTexture() {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // GL_NEAREST is vital for fractals to keep pixel edges sharp
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FractalComponent::UploadTexture() {
    if (m_textureID == 0 || m_pixelBuffer.empty()) return;

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixelBuffer.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

ImTextureID FractalComponent::GetResultTexture() const { 
    return (ImTextureID)(intptr_t)m_textureID; 
}