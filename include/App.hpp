#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "AmountSliderComponent.hpp"

class App {
public:
    enum class Channel { Red, Green, Blue };

private:
    std::map<Channel, std::unique_ptr<AmountSliderComponent>> m_components;
    Channel m_activeKey;

public:
    App();
    void Run();
};