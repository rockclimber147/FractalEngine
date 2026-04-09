#include "fractals/GPULyapunov.hpp"

GPULyapunov::GPULyapunov(std::string name) : GPUFractalComponent(std::move(name))
{
    UpdateTexture();
}

void GPULyapunov::UploadExtraUniforms() {
    glUniform1iv(glGetUniformLocation(m_shaderProgram, "u_sequence"), 16, m_sequence);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "u_seqLength"), m_seqLength);
    glUniform1i(glGetUniformLocation(m_shaderProgram, "u_maxIterations"), m_maxIterations);
}

void GPULyapunov::DrawControlPanel() {
    ImGui::Text("Lyapunov Sequence (A/B)");
    
    bool changed = false;
    for (int i = 0; i < m_seqLength; i++) {
        char label[16];
        sprintf(label, "%c##%d", (m_sequence[i] == 0 ? 'A' : 'B'), i);
        if (ImGui::Button(label)) {
            m_sequence[i] = (m_sequence[i] == 0 ? 1 : 0);
            changed = true;
        }
        ImGui::SameLine();
    }
    
    if (ImGui::Button("+") && m_seqLength < 16) { m_seqLength++; changed = true; }
    ImGui::SameLine();
    if (ImGui::Button("-") && m_seqLength > 2) { m_seqLength--; changed = true; }

    if (ImGui::SliderInt("Iterations", &m_maxIterations, 50, 500)) changed = true;

    if (changed) UpdateTexture();
}

