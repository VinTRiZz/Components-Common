#include "utility.hpp"

#include <stdexcept>
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>

#ifdef __linux__
#include <signal.h>
#include <termios.h>
#else
#include <windows.h> // TODO: Check it, one day when i even start Windows again
#endif // Linux

#include <cstring>

#include <boost/stacktrace.hpp>
#include <boost/core/demangle.hpp>

#include <Components/Logger/Logger.h>

namespace Common
{

void printStacktrace(int signo) {
    COMPLOG_ERROR_SYNC("SIGNAL:", signo, "(", strsignal(signo), ")");
    ::signal(signo, SIG_DFL);

    COMPLOG_EMPTY_SYNC("STACK TRACE:");
    COMPLOG_EMPTY_SYNC("====================================================");
    boost::stacktrace::stacktrace stackTrace;
    int traceLayer {0};
    for (auto& line : stackTrace) {
        COMPLOG_EMPTY_SYNC(traceLayer++, to_string(line));
    }

    COMPLOG_EMPTY_SYNC("====================================================");
    ::exit(-1);
}

static std::function<bool (int)> currentSignalProcessor;

void processSignal(int signo) {
    if (currentSignalProcessor && currentSignalProcessor(signo)) {
        return;
    }
    printStacktrace(signo);
    exit (1);
}

void setupBacktrace(std::function<bool (int)> &&signalProcessor)
{
    currentSignalProcessor = std::move(signalProcessor);
    ::signal(SIGSEGV, &processSignal);
    ::signal(SIGABRT, &processSignal);
    ::signal(SIGTERM, &processSignal);
    ::signal(SIGFPE, &processSignal);
}

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

std::pair<unsigned, unsigned> terminalGetXY()
{
    std::pair<unsigned, unsigned> res;
#ifdef __linux__
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Ask and get pos
    std::cout << "\033[6n" << std::flush;
    char buf[32];
    int i = 0;
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, buf + i, 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Восстанавливаем настройки

    sscanf(buf, "\033[%d;%d", &res.first, &res.second);
    return res;
#else
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);  // get console handle
    CONSOLE_SCREEN_BUFFER_INFO csbi;        // screen buffer information
    GetConsoleScreenBufferInfo(hc, &csbi);      // Get screen info & size
    res.first = csbi.dwCursorPosition.X;
    res.second = csbi.dwCursorPosition.Y;
    return res;
#endif // __linux__
}

int createRandomNumber(int min, int max)
{
    if (min >= max) throw std::invalid_argument("createRandomNumber: max >= min");
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}

std::string createRandomString(unsigned int stringLength) {
    std::random_device rd;
    std::mt19937 gen(rd());

    const std::string characters =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::uniform_int_distribution<> dis(0, characters.size() - 1);

    std::string result;
    result.reserve(stringLength);
    for (unsigned int i = 0; i < stringLength; ++i)
        result += characters[dis(gen)];

    return result;
}

uint64_t getEpoch()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
}

std::string getCurrentTimestampFormatted() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

    std::time_t now_c = std::chrono::system_clock::to_time_t(now_ms);
    std::tm now_tm;

    localtime_r(&now_c, &now_tm);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3)
        << (now_ms.time_since_epoch().count() % 1000);

    return oss.str();
}

}
