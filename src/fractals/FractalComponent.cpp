#include "fractals/FractalComponent.hpp"

FractalComponent::FractalComponent() {
    InitTexture();
}

FractalComponent::~FractalComponent() {
    if (m_textureID != 0) {
        glDeleteTextures(1, &m_textureID);
    }
}

/**
 * @brief Translates the viewport center based on pixel-space mouse movement.
 * * Converts a screen-space delta (pixels) into world-space coordinates, 
 * ensuring the movement speed is relative to the current zoom level 
 * and maintains the correct aspect ratio.
 * * @param dx Horizontal pixel displacement.
 * @param dy Vertical pixel displacement.
 * @param vW Current width of the viewport in pixels.
 * @param vH Current height of the viewport in pixels.
 */
void FractalComponent::Pan(float dx, float dy, float vW, float vH) {
    double ppu = GetPixelsPerUnit();

    m_offsetX -= (double)dx / ppu;
    m_offsetY -= (double)dy / ppu;

    UpdateTexture();
}

/**
 * @brief Zooms the viewport relative to a specific pixel coordinate.
 * * Adjusts the zoom level and shifts the offset so that the world-coordinate 
 * beneath the mouse cursor remains in the same screen-space position.
 * * @param amount Scroll delta (positive zooms in).
 * @param mouseX The X-coordinate of the mouse relative to the image panel.
 * @param mouseY The Y-coordinate of the mouse relative to the image panel.
 * @param vW Current width of the viewport in pixels.
 * @param vH Current height of the viewport in pixels.
 */
void FractalComponent::Zoom(float amount, float mouseX, float mouseY, float vW, float vH) {
    if (amount == 0) return;

    // 1. World-space point under mouse BEFORE zoom
    // Note: We use the same (mouseX - halfW) / ppu logic from UpdateTexture
    double ppuBefore = GetPixelsPerUnit();
    double mouseWorldX = m_offsetX + (mouseX - vW * 0.5) / ppuBefore;
    double mouseWorldY = m_offsetY + (mouseY - vH * 0.5) / ppuBefore;

    // 2. Apply the zoom factor
    double multiplier = (amount > 0) ? 1.1 : (1.0 / 1.1);
    m_zoom *= multiplier;

    // 3. Determine world-space point under mouse AFTER zoom
    double ppuAfter = GetPixelsPerUnit();
    double mouseWorldXAfter = m_offsetX + (mouseX - vW * 0.5) / ppuAfter;
    double mouseWorldYAfter = m_offsetY + (mouseY - vH * 0.5) / ppuAfter;

    // 4. Adjust the offset to "pin" the point
    m_offsetX += (mouseWorldX - mouseWorldXAfter);
    m_offsetY += (mouseWorldY - mouseWorldYAfter);

    UpdateTexture();
}

/**
 * @brief Resizes the internal pixel buffer and updates dimensions.
 * * Reallocates memory for the CPU-side pixel buffer only if the dimensions 
 * have changed. This keeps the rendering resolution in sync with the UI panel.
 * * @param w New width in pixels.
 * @param h New height in pixels.
 */
void FractalComponent::Resize(int w, int h) {
    if (w == m_width && h == m_height) return;
    if (w <= 0 || h <= 0) return;
    m_width = w;
    m_height = h;
    m_pixelBuffer.assign(m_width * m_height * 3, 0);
    UpdateTexture();
}

/**
 * @brief Generates and configures the OpenGL texture object.
 * * Sets up the texture with GL_NEAREST filtering to ensure that 
 * fractal pixels remain sharp and aliasing-free when scaled.
 */
void FractalComponent::InitTexture() {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // GL_NEAREST is vital for fractals to keep pixel edges sharp
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief Synchronizes the CPU pixel buffer with the GPU texture.
 * * Uploads the raw RGB data from the std::vector to the allocated 
 * OpenGL texture memory.
 */
void FractalComponent::UploadTexture() {
    if (m_textureID == 0 || m_pixelBuffer.empty()) return;

    glBindTexture(GL_TEXTURE_2D, m_textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pixelBuffer.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 * @brief Retrieves the OpenGL texture handle formatted for ImGui.
 * * @return ImTextureID The casted pointer to the OpenGL texture ID.
 */
ImTextureID FractalComponent::GetResultTexture() const { 
    return (ImTextureID)(intptr_t)m_textureID; 
}

void FractalComponent::UpdateTexture() {
    if (m_pixelBuffer.empty() || m_width <= 0 || m_height <= 0) return;

    double ppu = GetPixelsPerUnit();
    double halfW = m_width * 0.5;
    double halfH = m_height * 0.5;

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            // SHARED COORDINATE MAPPING
            double u = m_offsetX + (x - halfW) / ppu;
            double v = m_offsetY + (y - halfH) / ppu;

            unsigned char rgb[3];
            // CALL THE SUBCLASS LOGIC
            GetPixelColor(u, v, rgb);

            int idx = (y * m_width + x) * 3;
            m_pixelBuffer[idx + 0] = rgb[0];
            m_pixelBuffer[idx + 1] = rgb[1];
            m_pixelBuffer[idx + 2] = rgb[2];
        }
    }
    UploadTexture();
}