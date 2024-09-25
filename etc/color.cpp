#include <unordered_set>
#include <algorithm>
#include "color.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

std::map<int, Color> ColorManager::colors;

std::unordered_set<std::string> forbidden = {
    "CON", "PRN", "AUX", "NUL",
    "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9",
    "LPT1", "LPT2", "LPT3", "LPT4", "LPT5", "LPT6", "LPT7", "LPT8", "LPT9"
};
std::unordered_set<std::string> supportedExtensions = { // no .lst
    "cm", "cmap"
};

void ColorManager::LoadColors(std::string filename) {
    std::string filename2 = filename; // store the old filename (it'll change) a line below
    std::transform(filename.begin(), filename.end(), filename.begin(), ::toupper);

    size_t pos = 0;
    while ((pos = filename.find('/')) != std::string::npos) {
        std::string part = filename.substr(0, pos);
        if (forbidden.contains(part)) {
            std::cerr << "Color map file not found" << std::endl;
            exit(1);
        }
        filename.erase(0, pos + 1);
    }

    // remove the file extension & check whether file type is supported or not
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string extension = filename.substr(dotPos + 1);
        for (char& c : extension) c = tolower(c);
        if (!supportedExtensions.count(extension)) {
            std::cerr << "Wrong file extension" << std::endl;
            exit(1);
        }
        filename.erase(dotPos);
    }

    std::string filename3 = filename;

    // check the remaining part of the filename
    if (forbidden.contains(filename)) {
        std::cerr << "Color map file not found" << std::endl;
        exit(1);
    }

    filename = filename2;
    std::ifstream file(filename);
    std::string line;

    if (!file) {
        std::cerr << "Color map file not found" << std::endl;
        exit(1);
    }

    if (!std::getline(file, line) || line != "Define custom color mapping 1.0;") {
        std::cerr << "Invalid file format" << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '//') continue;

        std::istringstream iss(line);
        int code;
        std::string escape;
        if (iss >> code >> escape)
            colors.insert(std::make_pair(code, Color(code, escape)));
    }
}

Color ColorManager::getColor(int code) {
    auto it = colors.find(code);
    if (it != colors.end()) return it->second;
    std::cerr << "Unknown color " << std::to_string(code) << std::endl;
    return Color(0, ""); // default color
}