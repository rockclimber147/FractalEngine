#include "fractals/Lyapunov/GPULyapunov.hpp"

GPULyapunov::GPULyapunov(std::string name) : m_name(std::move(name))
{
    UpdateTexture();
}

std::string GPULyapunov::GetLabel() const { return m_name; }


const char* GPULyapunov::GetFragmentShaderSource() {
    return R"(
        #version 330 core
        #extension GL_ARB_gpu_shader_fp64 : enable 

        out vec4 FragColor;

        uniform dvec2 u_offset;
        uniform double u_zoom;
        uniform vec2 u_resolution;
        uniform int u_maxIterations;

        uniform int u_sequence[16]; 
        uniform int u_seqLength;

        void main() {
            double ppu = 200.0lf * u_zoom;
            double a = u_offset.x + (double(gl_FragCoord.x) - double(u_resolution.x) * 0.5lf) / ppu;
            double b = u_offset.y + (double(gl_FragCoord.y) - double(u_resolution.y) * 0.5lf) / ppu;

            double x = 0.5lf;
            double sumLogDeriv = 0.0lf;

            for(int i = 0; i < 50; i++) {
                double r = (u_sequence[i % u_seqLength] == 0) ? a : b;
                x = r * x * (1.0lf - x);
            }

            for(int i = 0; i < u_maxIterations; i++) {
                double r = (u_sequence[i % u_seqLength] == 0) ? a : b;
                x = r * x * (1.0lf - x);
                sumLogDeriv += log(abs(float(r * (1.0lf - 2.0lf * x))));
            }

            double lambda = sumLogDeriv / double(u_maxIterations);

            if (lambda < 0.0lf) {
                float val = float(abs(lambda));
                FragColor = vec4(val * 0.5, val * 0.2, val, 1.0); // Deep blues/purples
            } else {
                FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            }
        }
    )";   
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

