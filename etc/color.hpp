#pragma once

#include <string>
#include <map>

class Color
{
public:
    Color(int code, const std::string& escapeSequence) : code(code), escapeSequence(escapeSequence) {}

    constexpr int getCode() const { return code; }
    const std::string& getEscapeSequence() const { return escapeSequence; }

private:
    int code;
    std::string escapeSequence;
};

class ColorManager
{
public:
    const static void LoadColors(const std::string& filename);
    const static Color getColor(int code);

private:
    static std::map<int, Color> colors;
};