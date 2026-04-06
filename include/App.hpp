#pragma once
#include <memory>
#include <vector>
#include "AmountSliderComponent.hpp"

class App {
public:
    enum class ColorChannel {
            Red,
            Green,
            Blue
        };

private:
    AmountSliderComponent m_red;
    AmountSliderComponent m_green;
    AmountSliderComponent m_blue;

    ColorChannel m_activeChannel = ColorChannel::Red;

public:
    // Constructor with Initializer List
    App()
        : m_red("Red"),
          m_green("Green"),
          m_blue("Blue") 
    {}

    // Main UI Loop
    void Run();
};