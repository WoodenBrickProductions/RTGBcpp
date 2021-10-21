#ifndef CUSTOMLOGGER_HPP
#define CUSTOMLOGGER_HPP

#include <stdio.h>
#include <time.h>
#include <raylib.h>

void LogCustom(int msgType, const char *text, va_list args = nullptr);

#endif // CUSTOMLOGGER_HPP