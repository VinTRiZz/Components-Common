#pragma once

#include <string>
#include <map>
#include <stdint.h>
#include <functional>

namespace Common {

void printStacktrace(int signo);
void setupBacktrace(std::function<void(int)>&& signalProcessor);

void terminalGotoXY(int x,int y);
std::pair<int, int> terminalGetXY();

std::string createRandomString(unsigned int stringSize);

uint64_t getLinuxEpoch();

int generateNumber(int min, int max);

std::string getCurrentTimestampFormatted();

}
