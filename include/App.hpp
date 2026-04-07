#pragma once
#include <map>
#include <string>
#include <memory>
#include <vector>
#include "FractalComponent.hpp"

class App {
public:
    enum class Channel { Red, Green, Blue };

private:
    std::map<std::string, std::unique_ptr<FractalComponent>> m_components;
    std::string m_activeKey;
    bool m_needsUpdate = true;

public:
    App();
    void Run();
};