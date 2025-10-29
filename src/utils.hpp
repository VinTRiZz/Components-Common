#pragma once

#include <string>
#include <map>

namespace Common {

void terminalGotoXY(int x,int y);
std::pair<int, int> terminalGetXY();

std::string createRandomString(unsigned int stringSize);

uint64_t getLinuxEpoch();

int generateNumber(int min, int max);

std::string getCurrentTimestampFormatted();

}
