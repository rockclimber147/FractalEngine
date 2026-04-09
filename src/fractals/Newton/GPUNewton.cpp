#include "fractals/Newton/GPUNewton.hpp"

GPUNewton::GPUNewton(std::string name) 
    : GPUFractalComponent(std::move(name)) 
{
    // Start zoomed out to see the whole set
    m_zoom = 0.5; 
    UpdateTexture();
}

const char* GPUNewton::GetFragmentShaderSource() {
    return R"(
        #version 330 core
        #extension GL_ARB_gpu_shader_fp64 : enable 

        out vec4 FragColor;
        uniform dvec2 u_offset;
        uniform double u_zoom;
        uniform vec2 u_resolution; 

        dvec2 complexDiv(dvec2 a, dvec2 b) {
            double den = dot(b, b);
            return dvec2(dot(a, b), a.y * b.x - a.x * b.y) / den;
        }

        dvec2 complexMul(dvec2 a, dvec2 b) {
            return dvec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
        }

        void main() {
            double ppu = 200.0lf * u_zoom;
        
            dvec2 z = u_offset + (dvec2(gl_FragCoord.xy) - dvec2(u_resolution * 0.5f)) / ppu;

            int root = -1;
            int iter = 0;
            
            for(iter = 0; iter < 100; iter++) {
                dvec2 z2 = complexMul(z, z);
                dvec2 fz = complexMul(z2, z) - dvec2(1.0lf, 0.0lf);
                
                dvec2 dfz = 3.0lf * z2;
                
                if (dot(dfz, dfz) < 1e-12) break;
                z -= complexDiv(fz, dfz);

                double eps = 0.0001lf;
                if (length(z - dvec2(1.0lf, 0.0lf)) < eps) { root = 0; break; }
                if (length(z - dvec2(-0.5lf, 0.86602540378lf)) < eps) { root = 1; break; }
                if (length(z - dvec2(-0.5lf, -0.86602540378lf)) < eps) { root = 2; break; }
            }

            vec3 baseCol;
            if (root == 0)      baseCol = vec3(1.0, 0.3, 0.3); // Red
            else if (root == 1) baseCol = vec3(0.3, 1.0, 0.3); // Green
            else if (root == 2) baseCol = vec3(0.3, 0.3, 1.0); // Blue
            else                baseCol = vec3(0.0, 0.0, 0.0); // No convergence

            // Shading based on iteration count
            float brightness = pow(1.0f - float(iter) / 100.0f, 0.5f);
            FragColor = vec4(baseCol * brightness, 1.0);
        }
    )";
}

void GPUNewton::UploadExtraUniforms() {
}

void GPUNewton::DrawControlPanel() {
    bool changed = false;

    ImGui::Separator();
    ImGui::Text("Viewport Info:");
    ImGui::Text("Pos: (%.3f, %.3f)", m_offsetX, m_offsetY);
    ImGui::Text("Zoom: %.4f", m_zoom);
}

