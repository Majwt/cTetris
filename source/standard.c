#include "standard.h"

int modneg(int a, int b) {
	int m = a%b;
	if (m<0) {
		m+=b;
	}
	return m;
}
void printfd(const char* format, ...) {
	#if DEBUG
	printf("[DEBUG] ");
	va_list ap;
	va_start(ap,format);
	vprintf(format,ap);
	va_end(ap);
	#endif
}