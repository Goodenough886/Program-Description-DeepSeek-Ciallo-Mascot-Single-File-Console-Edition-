// Ciallo Mascot — a single-file, dependency-free C++17 console demo.
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

namespace {
struct Point { double x, y; };

std::pair<int, int> terminalSize() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        return {info.srWindow.Right - info.srWindow.Left + 1,
                info.srWindow.Bottom - info.srWindow.Top + 1};
    }
#else
    winsize size{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0 && size.ws_col && size.ws_row)
        return {size.ws_col, size.ws_row};
#endif
    return {80, 25};
}

int parseDimension(const char* text, const char* label, int lower, int upper) {
    std::string value(text);
    std::size_t end = 0;
    long number = 0;
    try { number = std::stol(value, &end); }
    catch (const std::exception&) { throw std::invalid_argument(std::string(label) + " must be an integer"); }
    if (end != value.size() || number < lower || number > upper)
        throw std::invalid_argument(std::string(label) + " must be between " +
                                    std::to_string(lower) + " and " + std::to_string(upper));
    return static_cast<int>(number);
}

bool insideStar(double x, double y) {
    constexpr double pi = 3.14159265358979323846;
    std::array<Point, 10> vertices{};
    for (int i = 0; i < 10; ++i) {
        const double angle = -pi / 2 + i * pi / 5;
        const double radius = (i % 2 == 0) ? 0.98 : 0.43;
        vertices[i] = {radius * std::cos(angle), radius * std::sin(angle)};
    }
    bool inside = false;
    for (int i = 0, j = 9; i < 10; j = i++) {
        const Point a = vertices[i], b = vertices[j];
        if ((a.y > y) != (b.y > y) &&
            x < (b.x - a.x) * (y - a.y) / (b.y - a.y) + a.x)
            inside = !inside;
    }
    return inside;
}

void draw(int width, int height, bool color) {
    // A character cell is roughly twice as tall as it is wide.
    const int artHeight = std::min(height - 2, (width - 2) / 2);
    const int artWidth = artHeight * 2;
    const double cx = (width - 1) / 2.0, cy = (height - 1) / 2.0;
    std::vector<std::string> canvas(height, std::string(width, ' '));
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            const double x = 2.0 * (col - cx) / artWidth;
            const double y = 2.0 * (row - cy) / artHeight;
            if (insideStar(x, y)) canvas[row][col] = '*';
        }
    }
    // Keep face details readable even at smaller terminal sizes.
    const int eyeRow = static_cast<int>(std::lround(cy - artHeight * 0.055));
    const int eyeOffset = std::max(2, static_cast<int>(std::lround(artWidth * 0.082)));
    const int faceCol = static_cast<int>(std::lround(cx));
    auto put = [&](int row, int col, char character) {
        if (row >= 0 && row < height && col >= 0 && col < width &&
            canvas[row][col] == '*') canvas[row][col] = character;
    };
    put(eyeRow, faceCol - eyeOffset, 'o');
    put(eyeRow, faceCol + eyeOffset, 'o');
    const int mouthRow = static_cast<int>(std::lround(cy + artHeight * 0.10));
    const std::string mouth = "\\___/";
    for (std::size_t i = 0; i < mouth.size(); ++i)
        put(mouthRow, faceCol - 2 + static_cast<int>(i), mouth[i]);

    if (color) std::cout << "\033[93m";
    for (const auto& line : canvas) {
        const auto last = line.find_last_not_of(' ');
        if (last == std::string::npos) std::cout << '\n';
        else std::cout << line.substr(0, last + 1) << '\n';
    }
    if (color) std::cout << "\033[0m";
    std::cout << "Ciallo~!  (o^_^o)\n";
}

void usage(const char* name) {
    std::cout << "Usage: " << name << " [--width N] [--height N] [--original] [--color] [--beep] [--help]\n"
              << "  Defaults: fit terminal (up to 88x32); size limits: 24..512 x 12..300.\n"
              << "  --original  Render the original 448x250 canvas (may scroll heavily).\n"
              << "  --color     Use ANSI yellow if your terminal supports it.\n"
              << "  --beep      Request a terminal bell (not a recorded Ciallo voice).\n";
}
} // namespace

int main(int argc, char** argv) {
    try {
        const auto [columns, rows] = terminalSize();
        int width = std::clamp(columns, 24, 88);
        int height = std::clamp(rows - 2, 12, 32);
        bool color = false, beep = false;
        for (int i = 1; i < argc; ++i) {
            const std::string arg = argv[i];
            if (arg == "--help" || arg == "-h") { usage(argv[0]); return 0; }
            if (arg == "--original") { width = 448; height = 250; }
            else if (arg == "--color") color = true;
            else if (arg == "--beep") beep = true;
            else if (arg == "--width" || arg == "--height") {
                if (++i >= argc) throw std::invalid_argument(arg + " requires a number");
                if (arg == "--width") width = parseDimension(argv[i], "width", 24, 512);
                else height = parseDimension(argv[i], "height", 12, 300);
            } else throw std::invalid_argument("unknown option: " + arg);
        }
        if (beep) std::cout << '\a';
        draw(width, height, color);
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << "\nUse --help for options.\n";
        return 1;
    }
}
