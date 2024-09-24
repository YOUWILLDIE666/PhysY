#include <iostream>
#include <complex>
#include <vector>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <cstdlib>
std::cerr << "Sorry, you're using software that isn't based on Windows (meaning that I can't use conio.h and windows.h)" << std::endl;
std::exit(1);
#endif

// im not using my lovely color.hpp, since this is not the case
enum class ColorCode {
    BLACK = 40,
};

constexpr int WIDTH = 120; // default: 80
constexpr int HEIGHT = 33; // default: 24
constexpr int BUFFER_SIZE = 250000;
constexpr int ATTRIBUTE_SIZE = 25;

// Function to clear the console
void cls() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 }; // top-left corner
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', BUFFER_SIZE, pos, &written);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, ATTRIBUTE_SIZE, pos, &written);
    SetConsoleCursorPosition(hConsole, pos);
}

// Function to draw the Mandelbrot set
void draw(const double xmin, const double xmax, const double ymin, const double ymax, const int maxIter) {
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

            if (iter == maxIter)
                output[y] += "\x1B[" + std::to_string(static_cast<int>(ColorCode::BLACK)) + "m ";
            else
                output[y] += "\x1B[48;5;" + std::to_string((iter % 8) + 1) + "m ";
        }
        output[y] += "\x1B[0m\n"; // reset
        };

    std::vector<std::thread> threads;
    for (int y = 0; y < HEIGHT; ++y) threads.emplace_back(drawRow, y);
    for (auto& thread : threads) thread.join();
    for (const auto& row : output) std::cout << row;
}

int main() {
    HWND hWnd = GetConsoleWindow();
    wchar_t title[] = L"PhysY 0.2.0 \t\t\t\t\t\t !!! UNREGISTERED FREE HYPERCAM 2 !!!";
    wchar_t aa[1024]{};

    for (int i = 0; i < wcslen(title); i++) {
        aa[i * 2] = title[i];
        aa[i * 2 + 1] = 0x2000;
    }
    aa[wcslen(title) * 2] = 0;

    SetWindowTextW(hWnd, aa);

    double xmin = -2.5;
    double xmax = 0.0;
    double ymin = -1.5;
    double ymax = 0.0;
    int maxIter = 1000;

    int div = 4;
    double mul = 1.0; // don't set LEQ 0.5, otherwise zoom out won't work

    while (true) {
        draw(xmin, xmax, ymin, ymax, maxIter);

        int command = _getch();
        switch (command) {
        case '=': // +
            xmin = (xmin + xmax) / 2 - (xmax - xmin) / div;
            xmax = (xmin + xmax) / 2 + (xmax - xmin) / div;
            ymin = (ymin + ymax) / 2 - (ymax - ymin) / div;
            ymax = (ymin + ymax) / 2 + (ymax - ymin) / div;
            break;
        case '-':
            xmin = (xmin + xmax) / 2 - (xmax - xmin) * mul;
            xmax = (xmin + xmax) / 2 + (xmax - xmin) * mul;
            ymin = (ymin + ymax) / 2 - (ymax - ymin) * mul;
            ymax = (ymin + ymax) / 2 + (ymax - ymin) * mul;
            break;
        case 0x48: // up arrow
            ymin -= (ymax - ymin) / div;
            ymax -= (ymax - ymin) / div;
            break;
        case 0x50: // down arrow
            ymin += (ymax - ymin) / div;
            ymax += (ymax - ymin) / div;
            break;
        case 0x4B: // left arrow
            xmin -= (xmax - xmin) / div;
            xmax -= (xmax - xmin) / div;
            break;
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