#include <iostream>
#include <complex>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <cstdlib>
std::cerr << "Sorry, you're using software that isn't based on Windows (meaning that I can't use conio.h and windows.h)" << std::endl;
std::exit(1);
#endif

void cls() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { 0, 0 }; // top-left corner
    DWORD written;
    FillConsoleOutputCharacter(hConsole, ' ', 80 * 250000, pos, &written);
    FillConsoleOutputAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE, 80 * 25, pos, &written);
    SetConsoleCursorPosition(hConsole, pos);
}

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 24;

void draw(double xmin, double xmax, double ymin, double ymax, int maxIter) {
#pragma omp parallel for
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            double real = xmin + (xmax - xmin) * x / WIDTH;
            double imag = ymin + (ymax - ymin) * y / HEIGHT;
            complex<double> c(real, imag);
            complex<double> z(0, 0);
            int iter = 0;

            while (abs(z) < 2 && iter < maxIter) {
                z = z * z + c;
                ++iter;
            }

            if (iter == maxIter)
                cout << "\x1B[40m ";
            else
                cout << "\x1B[48;5;" << (iter % 8) + 1 << "m "; // colored background
        }
        cout << "\x1B[0m\n"; // reset color
    }
}

int main() {
    HWND hWnd = GetConsoleWindow();
    wchar_t title[] = L"PhysY DEATH EDITION\t\t\t\t\t\t!!! UNREGISTERED FREE HYPERCAM 2 !!!";
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
    ///////////////////
    int div = 4;
    double mul = 1.0;//1.5;

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
        }
        cls();
    }

    return 0;
}