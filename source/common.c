#include "common.h"

int modneg(int a, int b) {
	int m = a % b;
	if (m < 0) {
		m += b;
	}
	return m;
}
void printfd(const char* format, ...) {
#if DEBUG
	printf("[DEBUG] ");
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
#endif
}
bool xor (bool a, bool b)
{
	return (a || b) && !(a && b);
}

int min(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	return b;
}
int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	return b;
}