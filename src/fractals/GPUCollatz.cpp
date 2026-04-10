#include "fractals/GPUCollatz.hpp"
#include <imgui.h>

GPUCollatz::GPUCollatz(std::string name) 
    : GPUFractalComponent(std::move(name))
{
    m_zoom = 1.0;
    UpdateTexture();
}

void GPUCollatz::UploadExtraUniforms() {
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_colorFrequency"), m_colorFrequency);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_colorOffset"), m_colorOffset);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "u_maxIterations"), m_maxIterations);

    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_realShift"), m_realShift);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_imagShift"), m_imagShift);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_distortion"), m_distortion);
}

void GPUCollatz::DrawControlPanel() {
    bool changed = false;

    if (ImGui::SliderInt("Iterations", &m_maxIterations, 1, 500)) changed = true;
    if (ImGui::SliderFloat("Real Shift", &m_realShift, 0.0f, 2.0f)) changed = true;
    if (ImGui::SliderFloat("Imag Shift", &m_imagShift, -1.0f, 1.0f)) changed = true;
    if (ImGui::SliderFloat("Distortion", &m_distortion, 0.05f, 0.5f)) changed = true;
    
    ImGui::Separator();
    if (ImGui::SliderFloat("Color Freq", &m_colorFrequency, 0.1f, 10.0f)) changed = true;
    if (ImGui::SliderFloat("Color Offset", &m_colorOffset, 0.0f, 6.28f)) changed = true;

    if (changed) UpdateTexture();
}