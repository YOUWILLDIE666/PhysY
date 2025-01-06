#pragma once

#include <string>
#include <map>

class Color
{
public:
    Color(int code, std::string escapeSequence) : code(code), escapeSequence(escapeSequence) {}

    constexpr int getCode() const { return code; }
    std::string getEscapeSequence() const { return escapeSequence; }

private:
    int code;
    std::string escapeSequence;
};

class ColorManager
{
public:
    const static void LoadColors(std::string_view filename);
    const static Color getColor(int code);

private:
    static std::map<int, Color> colors;
};