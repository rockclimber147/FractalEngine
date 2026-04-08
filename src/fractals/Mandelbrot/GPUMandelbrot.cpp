#include "fractals/Mandelbrot/GPUMandelbrot.hpp"

GPUMandelbrot::GPUMandelbrot(std::string name) 
    : m_name(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

std::string GPUMandelbrot::GetLabel() const { return m_name; }

const char* GPUMandelbrot::GetFragmentShaderSource() {
    return R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec2 u_offset;
        uniform float u_zoom;
        uniform vec2 u_resolution;
        uniform int u_maxIterations;
        uniform float u_colorFrequency;
        uniform float u_colorOffset;

        void main() {
            float ppu = 200.0 * u_zoom;
            float u = u_offset.x + (gl_FragCoord.x - u_resolution.x * 0.5) / ppu;
            float v = u_offset.y + (gl_FragCoord.y - u_resolution.y * 0.5) / ppu;

            vec2 c = vec2(u, v);
            vec2 z = vec2(0.0);
            int iter = 0;
            while(dot(z, z) <= 4.0 && iter < u_maxIterations) {
                z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
                iter++;
            }

            if (iter == u_maxIterations) {
                FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            } else {
                float t = float(iter) * (u_colorFrequency / 100.0);
                vec3 col = 0.5 + 0.5 * cos(3.0 + t + vec3(u_colorOffset, 2 + u_colorOffset, 4 + u_colorOffset));
                FragColor = vec4(col, 1.0);
            }
        }
    )";
}

void GPUMandelbrot::UploadExtraUniforms() {
    GLint maxIterLoc = glGetUniformLocation(m_shaderProgram, "u_maxIterations");
    GLint colorFreqLoc = glGetUniformLocation(m_shaderProgram, "u_colorFrequency");
    GLint colorOffsetLoc = glGetUniformLocation(m_shaderProgram, "u_colorOffset");

    glUniform1i(maxIterLoc, m_maxIterations);
    glUniform1f(colorFreqLoc, m_colorFrequency);
    glUniform1f(colorOffsetLoc, m_colorOffset);
}

void GPUMandelbrot::DrawControlPanel() {
    ImGui::Text("Mandelbrot");
    bool changed = false;

    if (ImGui::SliderInt("Detail (Iterations)", &m_maxIterations, 1, 2000)) {
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

