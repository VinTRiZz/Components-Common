#include "utils.hpp"

#include <stdexcept>
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>

#include <signal.h>
#include <cstring>

#include <boost/stacktrace.hpp>
#include <boost/core/demangle.hpp>

#include <Components/Logger/Logger.h>

namespace Common
{

#ifdef COMPONENTS_IS_ENABLED_QT
void initApplication(QApplication &a) {
    a.setApplicationName(PROJECT_NAME_STRING);
    a.setApplicationVersion(PROJECT_VERSION_STRING);
    a.setApplicationDisplayName(QString("%0 (версия %1)").arg(PROJECT_NAME_STRING, PROJECT_VERSION_STRING));
}
#endif // COMPONENTS_IS_ENABLED_QT

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

static std::function<void (int)> currentSignalProcessor;

void processSignal(int signo) {
    if (currentSignalProcessor) {
        currentSignalProcessor(signo);
        printStacktrace(signo);
        exit (0);
    }

    printStacktrace(signo);
    exit (1);
}

void setupBacktrace(std::function<void (int)> &&signalProcessor)
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

std::pair<int, int> terminalGetXY()
{
#ifdef __linux__
    return {};
#else
    HANDLE hc = GetStdHandle(STD_OUTPUT_HANDLE);  // get console handle
    CONSOLE_SCREEN_BUFFER_INFO csbi;        // screen buffer information
    GetConsoleScreenBufferInfo(hc, &csbi);      // Get screen info & size
    return std::make_pair(csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
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

std::string getCurrentTimestampFormatted() {
#ifdef QT_CORE
    return QDateTime::currentDateTime()
            .toString("yyyy-MM-dd_hh:mm:ss")
            .toStdString();
#else
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

    std::time_t now_c = std::chrono::system_clock::to_time_t(now_ms);
    std::tm now_tm;

    // Use localtime_s for thread safety (Windows)
#ifdef _WIN32
    localtime_s(&now_tm, &now_c);
#else
    // Use localtime_r for thread safety (POSIX)
    localtime_r(&now_c, &now_tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S")
        << '.' << std::setfill('0') << std::setw(3)
        << (now_ms.time_since_epoch().count() % 1000);

    return oss.str();
#endif // QT_CORE
}

}
