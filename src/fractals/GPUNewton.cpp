#include "fractals/GPUNewton.hpp"

GPUNewton::GPUNewton(std::string name) 
    : GPUFractalComponent(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

void GPUNewton::UploadExtraUniforms() {
    glUniform1i(glGetUniformLocation(m_shaderProgram, "u_maxIterations"), m_maxIterations);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_a"), m_a);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_b"), m_b);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_c"), m_c);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_d"), m_d);
}

void GPUNewton::DrawControlPanel() {
    bool changed = false;
    if (ImGui::SliderFloat("a (z^3)", &m_a, -2.0f, 2.0f)) changed = true;
    if (ImGui::SliderFloat("b (z^2)", &m_b, -2.0f, 2.0f)) changed = true;
    if (ImGui::SliderFloat("c (z)", &m_c, -2.0f, 2.0f)) changed = true;
    if (ImGui::SliderFloat("d (constant)", &m_d, -2.0f, 2.0f)) changed = true;
    if (changed) UpdateTexture();
}

