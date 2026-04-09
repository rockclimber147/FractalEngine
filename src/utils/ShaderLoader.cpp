#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include "utils/ShaderLoader.hpp"

namespace fs = std::filesystem;

std::string ShaderUtils::LoadShaderSource(const std::string& filename) {
    std::vector<fs::path> searchPaths = {
        fs::current_path() / "shaders" / filename,
        fs::current_path().parent_path() / "shaders" / filename
    };

    fs::path finalPath;
    bool found = false;

    for (const auto& p : searchPaths) {
        if (fs::exists(p)) {
            finalPath = p;
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "ERROR: Shader file not found: " << filename << std::endl;
        return "";
    }

    // 2. Read the file
    std::ifstream file(finalPath);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open shader file: " << finalPath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}