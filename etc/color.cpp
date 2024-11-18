#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <color.hpp>

std::map<int, Color> ColorManager::colors;

const std::unordered_set<std::string_view> forbidden = {
    "CON", "PRN", "AUX", "NUL",
    "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
    "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
};

const std::unordered_set<std::string_view> supportedExtensions = {
    "cm", "cmap"
};

void checkForbidden(const std::string_view name)
{
    if (forbidden.find(name) != forbidden.end())
    {
        throw std::runtime_error("Color map file not found");
    }
}

const void ColorManager::LoadColors(const std::string& filename)
{
    std::string filenameUpper = filename;
    std::transform(filenameUpper.begin(), filenameUpper.end(), filenameUpper.begin(), ::toupper);

    size_t pos = 0;
    while ((pos = filenameUpper.find('/')) != std::string::npos)
    {
        checkForbidden(filenameUpper.substr(0, pos));
        filenameUpper.erase(0, pos + 1);
    }

    size_t dotPos = filenameUpper.find_last_of('.');
    if (dotPos != std::string::npos)
    {
        std::string_view extension = filenameUpper.substr(dotPos + 1);
        std::string lowerExtension(extension);
        std::transform(lowerExtension.begin(), lowerExtension.end(), lowerExtension.begin(), ::tolower);
        if (supportedExtensions.find(lowerExtension) == supportedExtensions.end())
        {
            throw std::runtime_error("Wrong file extension");
        }
        filenameUpper.erase(dotPos);
    }

    checkForbidden(filenameUpper);

    std::ifstream file(filename);
    if (!file)
    {
        throw std::runtime_error("Color map file not found");
    }

    std::string line;
    if (!std::getline(file, line) || line != "Define custom color mapping 1.0;")
    {
        throw std::runtime_error("Invalid file format");
    }

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '/')
        {
            continue;
        }

        std::istringstream iss(line);
        int code;
        std::string escape;
        if (iss >> code >> escape)
        {
            colors.emplace(code, Color(code, escape));
        }
    }
}

const Color ColorManager::getColor(int code)
{
    auto it = colors.find(code);
    if (it != colors.end())
    {
        return it->second;
    }
    std::cerr << "Unknown color " << code << std::endl;
    return Color(0, ""); // default color
}