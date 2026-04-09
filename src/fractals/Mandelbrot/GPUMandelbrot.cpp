#include "fractals/Mandelbrot/GPUMandelbrot.hpp"

GPUMandelbrot::GPUMandelbrot(std::string name) 
    : GPUFractalComponent(std::move(name))
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

const char* GPUMandelbrot::GetFragmentShaderSource() {
    return R"(
        #version 330 core
        #extension GL_ARB_gpu_shader_fp64 : enable 

        out vec4 FragColor;

        uniform dvec2 u_offset;
        uniform double u_zoom;
        uniform vec2 u_resolution;
        uniform int u_maxIterations;
        uniform float u_colorFrequency;
        uniform float u_colorOffset;

        void main() {
            double ppu = 200.0lf * u_zoom;
            
            double u = u_offset.x + (double(gl_FragCoord.x) - double(u_resolution.x) * 0.5lf) / ppu;
            double v = u_offset.y + (double(gl_FragCoord.y) - double(u_resolution.y) * 0.5lf) / ppu;

            dvec2 c = dvec2(u, v);
            dvec2 z = dvec2(0.0lf);
            int iter = 0;

            while(dot(z, z) <= 4.0lf && iter < u_maxIterations) {
                z = dvec2(z.x*z.x - z.y*z.y, 2.0lf*z.x*z.y) + c;
                iter++;
            }

            if (iter == u_maxIterations) {
                FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            } else {
                float t = float(iter) * (u_colorFrequency / 100.0);
                vec3 col = 0.5 + 0.5 * cos(3.0 + t + vec3(u_colorOffset, 2.0 + u_colorOffset, 4.0 + u_colorOffset));
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
    bool changed = false;

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

