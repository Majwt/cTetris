#include "defines.h"
#include "special.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void printfd(const char* format, ...) {
    #if DEBUG
    va_list ap;
    va_start(ap,format);
    char buffer[1000];
    const char debug[] = "[DEBUG] ";
    vsprintf(buffer,format,ap);
    // strcat(printbuffer,buffer);
    // strcat(printbuffer,"\n");
    va_end(ap);
    printf(debug);
    printf(buffer);
    #endif
}