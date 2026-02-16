#pragma once

#include <string>
#include <map>
#include <stdint.h>
#include <functional>

namespace Common {

#ifdef COMPONENTS_IS_ENABLED_QT
void initApplication(QApplication& a);
#endif // COMPONENTS_IS_ENABLED_QT

void printStacktrace(int signo);
void setupBacktrace(std::function<void(int)>&& signalProcessor);

void terminalGotoXY(int x,int y);
std::pair<int, int> terminalGetXY();

std::string createRandomString(unsigned int stringSize);

uint64_t getLinuxEpoch();

int generateNumber(int min, int max);

std::string getCurrentTimestampFormatted();

}
