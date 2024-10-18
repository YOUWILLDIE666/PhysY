#include "etc/color.hpp"

#include <iostream>
#include <complex>
#include <vector>
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <mutex>

bool aaa = false;

constexpr int WIDTH = 120;
constexpr int HEIGHT = 40;
constexpr int BUFFER_SIZE = 250000;
constexpr int ATTRIBUTE_SIZE = 25;

// im not using my lovely color.hpp, since this is not the case
enum class ColorCode {
    BLACK = 40,
};

std::mutex output_mutex;

int curIter = 0;

static void cls() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 }; // top-left corner
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', BUFFER_SIZE, pos, &written);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, ATTRIBUTE_SIZE, pos, &written);
    SetConsoleCursorPosition(hConsole, pos);
}

static std::string debuginfo(const double& xmin, const double& xmax, const double& ymin, const double& ymax, const int& maxIter) {
    std::stringstream ss;
    if (curIter < 0) curIter = 0; // ?
    ss << "Zoom: " << (xmax - xmin) << "x" << (ymax - ymin) << ", Iterations: " << curIter;
    return ss.str();
}

static void draw(const double xmin, const double xmax, const double ymin, const double ymax, const int maxIter) {
    std::vector<std::string> output(HEIGHT);

    auto drawRow = [&](int y) {
        for (int x = 0; x < WIDTH; ++x) {
            double real = xmin + (xmax - xmin) * x / WIDTH;
            double imag = ymin + (ymax - ymin) * y / HEIGHT;
            std::complex<double> c(real, imag);
            std::complex<double> z(0, 0);
            int iter = 0;

            while (std::abs(z) < 2 && iter < maxIter) {
                z = z * z + c;
                ++iter;
            }

            if (iter == maxIter) {
                output[y] += std::string("\x1B[") + std::to_string(static_cast<int>(ColorCode::BLACK)) + "m ";
            } else {
                if (!aaa) {
                    output[y] += std::string("\x1B[48;5;") + std::to_string((iter % 8) + 1) + "m ";
                } else {
                    Color color = ColorManager::getColor((iter % 8) + 1);
                    output[y] += std::string("\x1B[") + color.getEscapeSequence() + "m ";
                }
            }
        }
        output[y] += "\x1B[0m\n"; // reset
    };

    std::vector<std::thread> threads;
    for (int y = 0; y < HEIGHT; ++y) threads.emplace_back(drawRow, y);
    for (auto& thread : threads) thread.join();

    ///
    HWND hWnd = GetConsoleWindow();
    wchar_t title[1024]{};
    std::string debugInfo = "PhysY 0.3.0 (" + debuginfo(xmin, xmax, ymin, ymax, maxIter) + ")";
    //std::string debugInfo = std::string(avqa);
    wchar_t aa[1024]{};
    std::wstring ws(debugInfo.begin(), debugInfo.end());
    wcscpy_s(aa, 1024, ws.c_str());
    SetWindowTextW(hWnd, aa);
    ///

    for (const auto& row : output) std::cout << row;
}

int main() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
#endif
    std::string filename;
    std::cout << "If you have custom color mapping, enter it's path right here (skip otherwise): ";
    std::getline(std::cin, filename);
    if (!(filename.empty() || filename.find_first_not_of(" \t\r\f\v") == std::string::npos)) aaa = true;
    if (aaa) ColorManager::LoadColors(filename);
    cls();

    double xmin = -2.5;
    double xmax = 0.5; // LEQ 0.5 won't let you zoom out, plus it's no actually point to keep it like that
    double ymin = -1.5;
    double ymax = 0.5; // same
    int maxIter = 1000;

    int div = 4;
    double mul = 1.0; // don't set LEQ 0.5, otherwise zoom out won't work

    while (true) {
        draw(xmin, xmax, ymin, ymax, maxIter);

        int command = _getch();
        switch (command) {
        case '+':
        case '=':
            xmin = (xmin + xmax) / 2 - (xmax - xmin) / div;
            xmax = (xmin + xmax) / 2 + (xmax - xmin) / div;
            ymin = (ymin + ymax) / 2 - (ymax - ymin) / div;
            ymax = (ymin + ymax) / 2 + (ymax - ymin) / div;
            ++curIter;
            break;
        case '-':
            xmin = (xmin + xmax) / 2 - (xmax - xmin) * mul;
            xmax = (xmin + xmax) / 2 + (xmax - xmin) * mul;
            ymin = (ymin + ymax) / 2 - (ymax - ymin) * mul;
            ymax = (ymin + ymax) / 2 + (ymax - ymin) * mul;
            ++curIter;
            break;
        case 0x57: // W
        case 0x77: // w
        case 0x48: // up arrow
            ymin -= (ymax - ymin) / div;
            ymax -= (ymax - ymin) / div;
            break;
        case 0x53: // S
        case 0x73: // s
        case 0x50: // down arrow
            ymin += (ymax - ymin) / div;
            ymax += (ymax - ymin) / div;
            break;
        case 0x41: // A
        case 0x61: // a
        case 0x4B: // left arrow
            xmin -= (xmax - xmin) / div;
            xmax -= (xmax - xmin) / div;
            break;
        case 0x44: // D
        case 0x64: // d
        case 0x4D: // right arrow
            xmin += (xmax - xmin) / div;
            xmax += (xmax - xmin) / div;
            break;
        default:
            break;
        }
        cls();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
