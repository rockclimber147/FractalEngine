#pragma once
#include <memory>
#include <vector>

class Mandelbrot;

class App {
public:
    App();
    void Run(); // The main loop

private:
    float m_brightness = 1.0f;
};