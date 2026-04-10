#include "fractals/core/FractalComponent.hpp"

FractalComponent::FractalComponent(std::string name) 
    : m_name(std::move(name))
{
}

FractalComponent::~FractalComponent() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

void FractalComponent::Pan(float dx, float dy, float vW, float vH) {
    double ppu = GetPixelsPerUnit();
    
    m_offsetX -= (double)dx / ppu;
    m_offsetY += (double)dy / ppu; 
    
    UpdateTexture();
}

void FractalComponent::Zoom(float amount, float mouseX, float mouseY, float vW, float vH) {
    if (amount == 0) return;
    
    float correctedMouseY = vH - mouseY;

    double ppuBefore = GetPixelsPerUnit();
    double mouseWorldX = m_offsetX + (mouseX - vW * 0.5) / ppuBefore;
    double mouseWorldY = m_offsetY + (correctedMouseY - vH * 0.5) / ppuBefore;

    double multiplier = (amount > 0) ? 1.1 : (1.0 / 1.1);
    m_zoom *= multiplier;

    double ppuAfter = GetPixelsPerUnit();
    double mouseWorldXAfter = m_offsetX + (mouseX - vW * 0.5) / ppuAfter;
    double mouseWorldYAfter = m_offsetY + (correctedMouseY - vH * 0.5) / ppuAfter;

    m_offsetX += (mouseWorldX - mouseWorldXAfter);
    m_offsetY += (mouseWorldY - mouseWorldYAfter);
    
    UpdateTexture();
}

ImTextureID FractalComponent::GetResultTexture() const { 
    return (ImTextureID)(intptr_t)m_textureID; 
}

std::string FractalComponent::GetLabel() const {
    return m_name;
}

void FractalComponent::DrawViewportInfo() {
    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}