#include "utils.hpp"

#include <stdexcept>
#include <chrono>
#include <random>

namespace CommonFunctions
{

void terminalGotoXY(int x, int y)
{
#ifdef __linux__
    printf("%c[%d;%df",0x1B,y,x);
#else
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);  // get console handle
    COORD cursor = { x, y };
    SetConsoleCursorPosition(hc, cursor);  // set new cursor position
#endif // __linux__
}

std::string createRandomString(unsigned int stringSize) {
    std::random_device rd;
    std::mt19937 gen(rd());

    const std::string characters =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string result;
    result.reserve(stringSize);
    for (unsigned int i = 0; i < stringSize; ++i)
        result += characters[dis(gen)];

    return result;
}

uint64_t getLinuxEpoch()
{
#ifdef __linux__
    std::chrono::system_clock::time_point now =
            std::chrono::system_clock::now();

    std::time_t now_t = std::chrono::system_clock::to_time_t(now);

    return static_cast<int64_t>(now_t);
#else
    return 0;
#endif // Linux
}

int generateNumber(int min, int max)
{
    if (min >= max) throw std::invalid_argument("generateNumber: max >= min");

    // Create a random number generator
    std::random_device rd;
    std::mt19937 rng(rd());

    // Define the distribution
    std::uniform_int_distribution<int> uni(min, max);

    // Generate and return the random number
    return uni(rng);
}

}
