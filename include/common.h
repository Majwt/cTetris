#ifndef common_h
#define common_h
#include "defines.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

void printfd(const char* format, ...);
int modneg(int a, int b);
bool xor (bool a, bool b);
int min(int a, int b);
int max(int a, int b);


#endif