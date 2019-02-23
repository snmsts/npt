#include "c99.h"
#include <stdio.h>
#include <stdarg.h>
#include <locale.h>

const char *setlocale_c(int category)
{
	const char *ptr;

	ptr = setlocale(category, NULL);
	if (ptr && (ptr[0] != 'C' || ptr[1] != 0)) {
		return setlocale(category, "C");
	}

	return NULL;
}

int vsnprintc(char *buffer, size_t size, const char *fmt, va_list args)
{
	int result;
	const char *check;

	/* setlocale C */
	check = setlocale_c(LC_NUMERIC);

	/* printf */
	result = vsnprintf(buffer, size, fmt, args);
	if (result <= 0)
		buffer[size - 1] = 0;

	/* setlocale */
	if (check)
		setlocale(LC_NUMERIC, check);

	return result;
}

int snprintc(char *buffer, size_t size, const char *fmt, ...)
{
	int result;
	va_list args;

	va_start(args, fmt);
	result = vsnprintc(buffer, size, fmt, args);
	va_end(args);

	return result;
}
