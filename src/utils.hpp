#pragma once

#include <string>

namespace CommonFunctions
{

void terminalGotoXY(int x,int y);

std::string createRandomString(unsigned int stringSize);

uint64_t getLinuxEpoch();

int generateNumber(int min, int max);

}
