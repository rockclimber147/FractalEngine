#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "AmountSliderComponent.hpp"

class App {
private:
    std::map<std::string, std::unique_ptr<AmountSliderComponent>> m_components;
    std::string m_activeKey;

public:
    App();
    void Run();
};