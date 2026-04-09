#include "fractals/GPUApollonianGasket.hpp"

GPUApollonianGasket::GPUApollonianGasket(std::string name) : GPUFractalComponent(std::move(name))
{
    m_zoom = 0.5;
    UpdateTexture();
}

void GPUApollonianGasket::UploadExtraUniforms() {
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_colorFrequency"), m_colorFrequency);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_colorOffset"), m_colorOffset);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_foldScale"), m_foldScale);
    glUniform1f(glGetUniformLocation(m_shaderProgram, "u_spread"), m_spread);
}

void GPUApollonianGasket::DrawControlPanel() {
    bool changed = false;

    ImGui::Text("Fractal Geometry");
    if (ImGui::SliderFloat("Fold Scale", &m_foldScale, 0.1f, 2.0f)) changed = true;
    if (ImGui::SliderFloat("Spread", &m_spread, 0.5f, 3.0f)) changed = true;

    ImGui::Separator();
    ImGui::Text("Color Settings");
    if (ImGui::SliderFloat("Frequency", &m_colorFrequency, 0.1f, 10.0f)) changed = true;
    if (ImGui::SliderFloat("Hue Offset", &m_colorOffset, 0.0f, 6.28f)) changed = true;

    if (changed) UpdateTexture();
}