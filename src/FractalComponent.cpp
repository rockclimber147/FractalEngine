#include "FractalComponent.hpp"

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
    double aspect = (double)vW / vH;
    m_offsetX -= (dx / vW) * (4.0 / m_zoom) * aspect;
    m_offsetY -= (dy / vH) * (4.0 / m_zoom);
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

    double aspect = (double)vW / vH;

    // 1. Determine the world-space coordinate under the mouse before zooming
    double beforeX = m_offsetX + ((mouseX / (double)vW) - 0.5) * (4.0 / m_zoom) * aspect;
    double beforeY = m_offsetY + ((mouseY / (double)vH) - 0.5) * (4.0 / m_zoom);

    // 2. Apply the zoom factor
    double multiplier = (amount > 0) ? 1.1 : (1.0 / 1.1);
    m_zoom *= multiplier;

    // 3. Determine where that same world-space point is after zooming
    double afterX = m_offsetX + ((mouseX / (double)vW) - 0.5) * (4.0 / m_zoom) * aspect;
    double afterY = m_offsetY + ((mouseY / (double)vH) - 0.5) * (4.0 / m_zoom);

    // 4. Adjust the offset by the difference to "pin" the point under the mouse
    m_offsetX += (beforeX - afterX);
    m_offsetY += (beforeY - afterY);

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