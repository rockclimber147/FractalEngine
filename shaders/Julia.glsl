#version 330 core
#extension GL_ARB_gpu_shader_fp64 : enable 

out vec4 FragColor;

uniform dvec2 u_offset;
uniform double u_zoom;
uniform vec2 u_resolution;
uniform int u_maxIterations;
uniform float u_colorFrequency;
uniform float u_colorOffset;
uniform dvec2 u_juliaC;

void main() {
    double ppu = 200.0lf * u_zoom;
    
    double u = u_offset.x + (double(gl_FragCoord.x) - double(u_resolution.x) * 0.5lf) / ppu;
    double flippedY = double(u_resolution.y) - double(gl_FragCoord.y);
    double v = u_offset.y + (flippedY - double(u_resolution.y) * 0.5lf) / ppu;

    dvec2 z = dvec2(u, v);    // Pixel position is starting Z
    dvec2 c = u_juliaC;       // C is the fixed constant
    
    int iter = 0;
    while(dot(z, z) <= 4.0lf && iter < u_maxIterations) {
        z = dvec2(z.x*z.x - z.y*z.y, 2.0lf*z.x*z.y) + c;
        iter++;
    }

    if (iter == u_maxIterations) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float t = float(iter) * (u_colorFrequency / 100.0);
        // Coloring logic stays float-based (it's post-calculation)
        vec3 col = 0.5 + 0.5 * cos(3.0 + t + vec3(u_colorOffset, 2.0 + u_colorOffset, 4.0 + u_colorOffset));
        FragColor = vec4(col, 1.0);
    }
}