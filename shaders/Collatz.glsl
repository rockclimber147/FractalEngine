#version 400 core
#extension GL_ARB_gpu_shader_fp64 : enable 

out vec4 FragColor;

uniform dvec2 u_offset;
uniform double u_zoom;
uniform vec2 u_resolution;
uniform int u_maxIterations;
uniform float u_colorFrequency;
uniform float u_colorOffset;

uniform float u_realShift;
uniform float u_imagShift;
uniform float u_distortion;

dvec2 c_mul(dvec2 a, dvec2 b) {
    return dvec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

dvec2 c_cos(dvec2 z) {
    float x = float(z.x);
    float y = float(z.y);
    return dvec2(double(cos(x) * cosh(y)), double(-sin(x) * sinh(y)));
}

void main() {
    double ppu = 200.0lf * u_zoom;

    dvec2 flippedCoord = dvec2(gl_FragCoord.x, double(u_resolution.y) - gl_FragCoord.y);
    dvec2 z = u_offset + (flippedCoord - dvec2(u_resolution * 0.5f)) / ppu;

    int iter = 0;
    double pi = 3.141592653589793lf;
    dvec2 shift = dvec2(double(u_realShift), double(u_imagShift));

    while(dot(z, z) < 10000.0lf && iter < u_maxIterations) {
        dvec2 pi_z = z * pi;
        dvec2 cos_pi_z = c_cos(pi_z);
        
        dvec2 term1 = 1.0lf + 4.0lf * z;
        dvec2 term2 = c_mul((shift + 2.0lf * z), cos_pi_z);
        
        z = double(u_distortion) * (term1 - term2);
        iter++;
    }

    if (iter == u_maxIterations) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        float zn2 = float(dot(z, z));
        float t = (float(iter) - log2(log2(zn2))) * (u_colorFrequency * 0.1);
        vec3 col = 0.5 + 0.5 * cos(3.0 + t + vec3(u_colorOffset, 2.0 + u_colorOffset, 4.0 + u_colorOffset));
        FragColor = vec4(col, 1.0);
    }
}