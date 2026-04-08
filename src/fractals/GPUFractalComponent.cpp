#include "fractals/GPUFractalComponent.hpp"

GPUFractalComponent::GPUFractalComponent() : FractalComponent() {}

GPUFractalComponent::~GPUFractalComponent() {
    if (m_fbo) glDeleteFramebuffers(1, &m_fbo);
    if (m_shaderProgram) glDeleteProgram(m_shaderProgram);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
}

void GPUFractalComponent::InitGLResources() {
    // Standard Vertex Shader (just passes coordinates)
    const char* vsSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        void main() { gl_Position = vec4(aPos, 0.0, 1.0); }
    )";

    // Compile logic
    auto compile = [](GLenum type, const char* source) {
        GLuint s = glCreateShader(type);
        glShaderSource(s, 1, &source, NULL);
        glCompileShader(s);
        // Error checking omitted for brevity, but recommended!
        return s;
    };

    GLuint vs = compile(GL_VERTEX_SHADER, vsSource);
    GLuint fs = compile(GL_FRAGMENT_SHADER, GetFragmentShaderSource());

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vs);
    glAttachShader(m_shaderProgram, fs);
    glLinkProgram(m_shaderProgram);

    // Quad Setup
    float vertices[] = { -1,1, -1,-1, 1,-1, 1,1 };
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // FBO Setup
    glGenFramebuffers(1, &m_fbo);
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureID, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GPUFractalComponent::UpdateTexture() {
    if (m_width <= 0 || m_height <= 0) return;
    if (!m_shaderProgram) InitGLResources(); // Lazy Init

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
    glUseProgram(m_shaderProgram);

    glUniform2d(glGetUniformLocation(m_shaderProgram, "u_offset"), m_offsetX, m_offsetY);
    glUniform1d(glGetUniformLocation(m_shaderProgram, "u_zoom"), m_zoom);
    glUniform2f(glGetUniformLocation(m_shaderProgram, "u_resolution"), (float)m_width, (float)m_height);

    UploadExtraUniforms();

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GPUFractalComponent::Resize(int w, int h) {
    if (w <= 0 || h <= 0 || (w == m_width && h == m_height)) return;
    m_width = w; m_height = h;
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    UpdateTexture();
}