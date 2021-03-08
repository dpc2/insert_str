
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdarg.h>

static void my_print(const char *func, int line, const char *file, char *fmt, ...)
{
	char str[4096] = {0};
	sprintf(str, "--%-20s--%5d--%-20s-- ", func, line, file);

	va_list args;
	va_start (args, fmt);
	vsnprintf(str + strlen(str), 4096 - 1 - strlen(str), fmt, args);
	va_end(args);

	sprintf(str + strlen(str), "\n");
	fputs(str, stdout);
	fflush(stdout);
}
