#pragma once
#include <memory>
#include <vector>
#include "AmountSliderComponent.hpp"

class App {
private:
    AmountSliderComponent m_red;
    AmountSliderComponent m_green;
    AmountSliderComponent m_blue;

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