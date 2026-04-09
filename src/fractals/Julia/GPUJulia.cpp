#include "fractals/Julia/GPUJulia.hpp"

GPUJulia::GPUJulia(std::string name) 
    : GPUFractalComponent(std::move(name))
{
    juliaCReal = -0.7f; 
    juliaCImag = 0.27f;
    m_zoom = 0.5; 
    m_presets = {
        {"Custom", 0.0f, 0.0f},
        {"The Rabbit", -0.123f, 0.745f},
        {"Dendrite", 0.0f, 0.641f},
        {"San Marco", -0.75f, 0.0f},
        {"The Dragon", 0.3602f, 0.10032f},
        {"Siegel Disk", -0.391f, -0.587f},
        {"Galactic", 0.285f, 0.01f}
    };
    UpdateTexture();
}

void GPUJulia::UploadExtraUniforms() {
    GLint juliaCLoc = glGetUniformLocation(m_shaderProgram, "u_juliaC");
    GLint maxIterLoc = glGetUniformLocation(m_shaderProgram, "u_maxIterations");
    GLint colorFreqLoc = glGetUniformLocation(m_shaderProgram, "u_colorFrequency");
    GLint colorOffsetLoc = glGetUniformLocation(m_shaderProgram, "u_colorOffset");

    glUniform2d(juliaCLoc, (double)juliaCReal, (double)juliaCImag);
    glUniform1i(maxIterLoc, m_maxIterations);
    glUniform1f(colorFreqLoc, m_colorFrequency);
    glUniform1f(colorOffsetLoc, m_colorOffset);
}

void GPUJulia::DrawControlPanel() {
    bool changed = false;

    ImGui::Text("Presets");
    if (ImGui::BeginCombo("Points of Interest", m_presets[m_selectedPreset].name.c_str())) {
        for (int i = 0; i < m_presets.size(); i++) {
            bool isSelected = (m_selectedPreset == i);
            if (ImGui::Selectable(m_presets[i].name.c_str(), isSelected)) {
                m_selectedPreset = i;
                
                // If it's not "Custom", apply the preset values
                if (m_selectedPreset != 0) {
                    juliaCReal = m_presets[m_selectedPreset].real;
                    juliaCImag = m_presets[m_selectedPreset].imag;
                    changed = true;
                }
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    ImGui::Text("Constant (c)");
    if (ImGui::DragFloat("Real", &juliaCReal, 0.001f, -2.0f, 2.0f)) {
        m_selectedPreset = 0; // Switch to Custom if tweaked
        changed = true;
    }
    if (ImGui::DragFloat("Imaginary", &juliaCImag, 0.001f, -2.0f, 2.0f)) {
        m_selectedPreset = 0;
        changed = true;
    }

    ImGui::Separator();

    if (ImGui::SliderInt("Max Iterations", &m_maxIterations, 1, 2000)) {
        changed = true;
    }

    if (ImGui::SliderFloat("Color Cycle Speed", &m_colorFrequency, 1.0f, 100.0f)) {
        changed = true;
    }

    if (ImGui::SliderFloat("Color Offset", &m_colorOffset, 0.0f, 6.0f)) {
        changed = true;
    }

    if (changed) {
        UpdateTexture();
    }

    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

