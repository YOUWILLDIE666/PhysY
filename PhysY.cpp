#include <color.hpp>
#include <parser.hpp>
#include <Windows.h>
#include <complex>
#include <thread>
#include <chrono>
#include <conio.h>
#include <mutex>

bool aaa = false;

constexpr int WIDTH = 120;
constexpr int HEIGHT = 40;

enum class ColorCode : std::uint8_t
{
    BLACK = 40
};

std::mutex output_mutex;

int curIter = 0;

static void cls()
{
    std::cout << "\x1B[2J\x1B[H";
}

static std::string debuginfo(double xmin, double xmax, double ymin, double ymax)
{
    std::stringstream ss;
    ss << "Zoom: " << (xmax - xmin) << "x" << (ymax - ymin) << ", Iterations: " << curIter;
    return ss.str();
}

static void draw(const double xmin, const double xmax, const double ymin, const double ymax, const int maxIter, std::string_view expression)
{
    std::vector<std::string> output(HEIGHT);
    std::vector<std::thread> threads;

    auto drawRow = [&](int y) -> void
    {
        std::string& row = output[y];
        for ( int x = 0; x < WIDTH; ++x )
        {
            double real = xmin + (xmax - xmin) * x / WIDTH;
            double imag = ymin + (ymax - ymin) * y / HEIGHT;
            std::complex<double> c(real, imag);
            std::complex<double> z(0, 0);
            int iter = 0;

            Interpreter interpreter({});
            interpreter.assign("c", c.real());  // Assign the real part of c
            interpreter.assign("ci", c.imag()); // Assign the imaginary part of c

            while ( std::abs(z) < 2 && iter < maxIter )
            {
                interpreter.assign("z", z.real());
                interpreter.assign("zi", z.imag());

                double z_real = z.real();
                double z_imag = z.imag();
                
                /*z = std::complex<double>(z_real * z_real - z_imag * z_imag, 
                             2 * z_real * z_imag) + c;*/

                z = interpreter.eval();

                ++iter;
            }

            if (iter == maxIter)
            {
                row += "\x1B[" + std::to_string(static_cast<int>(ColorCode::BLACK)) + "m ";
            }
            else
            {
                if (!aaa)
                {
                    row += "\x1B[48;5;" + std::to_string((iter % 8) + 1) + "m ";
                }
                else
                {
                    Color color = ColorManager::getColor((iter % 8) + 1);
                    row += "\x1B[" + color.getEscapeSequence() + "m ";
                }
            }
        }
        row += "\x1B[0m\n"; // reset
//        std::lock_guard<std::mutex> lock(output_mutex);
//        output[y] = row;
    };

    const int numThreads = std::thread::hardware_concurrency();
    for ( int i = 0; i < numThreads; ++i )
    {
        threads.emplace_back([&, i]()
        {
            for ( int y = i; y < HEIGHT; y += numThreads )
            {
                drawRow(y);
            }
        });
    }

    for (std::thread& thread : threads)
    {
        thread.join();
    }

    HWND hWnd = GetConsoleWindow();
    std::string debugInfoStr = debuginfo(xmin, xmax, ymin, ymax); // Call debuginfo once
    std::wstring title = L"PhysY 0.5.0 (" + std::wstring(debugInfoStr.begin(), debugInfoStr.end()) + L")"; // Use the stored result
    SetWindowTextW(hWnd, title.c_str());

    for (std::string_view row : output)
    {
        std::cout << row;
    }
}

std::optional<std::string> input(std::istream& is)
{
    std::string line;
    if (std::getline(is, line))
    {
        return line;
    }
    return std::nullopt;
}

int main()
{
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, dwMode);
#endif

    std::cout << "If you have a .cm file, enter its path right here (skip otherwise): ";
    std::optional<std::string> filename = input(std::cin);
    if (filename.has_value() && !filename.value().empty())
    {
        aaa = true;
        std::string& val = filename.value();
        ColorManager::LoadColors(val);
    }
    cls();

    double xmin = -2.5;
    double xmax = 0.5;
    double ymin = -1.5;
    double ymax = 0.5;
    int maxIter = 1000;

    int div = 4;
    double mul = 1.0;

    std::cout << "Enter a mathematical expression for the fractal (e.g., 'z^2 + c'): ";
    std::string expression;
    std::getline(std::cin, expression);

    auto start = std::chrono::high_resolution_clock::now();
    while (true)
    {
        draw(xmin, xmax, ymin, ymax, maxIter, expression);

        int command = _getch();
        switch (command)
        {
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

        auto end = std::chrono::high_resolution_clock::now();
        std::int64_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = end;
        std::this_thread::sleep_for(std::chrono::milliseconds(16 - duration)); // 16ms = 60fps
    }

    return 0;
}