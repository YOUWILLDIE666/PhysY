#pragma once

#include <string>
#include <map>

class Color {
public:
    Color(int code, const std::string& escapeSequence) : code(code), escapeSequence(escapeSequence) {}

    int getCode() const { return code; }
    const std::string& getEscapeSequence() const { return escapeSequence; }

private:
    int code;
    std::string escapeSequence;
};

class ColorManager {
public:
    static void LoadColors(std::string filename);
    static Color getColor(int code);

private:
    static std::map<int, Color> colors;
};