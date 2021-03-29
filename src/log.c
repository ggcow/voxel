#define _POSIX_C_SOURCE 200809L

#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

enum _verbosity {
	_VERBOSITY_ERROR = 0,
	_VERBOSITY_WARNING = 1,
	_VERBOSITY_INFO = 2,
	_VERBOSITY_DEBUG = 3,
};

const enum _verbosity _VERBOSITY = _VERBOSITY_DEBUG;

static const char *verbosity_prefixes[4][2] = {
	[_VERBOSITY_ERROR] = {
		[0] = "[Error]",
		[1] = "\x1B[1;31m[Error]\x1B[0m",
	},
	[_VERBOSITY_WARNING] = {
		[0] = "[Warning]",
		[1] = "\x1B[1;33m[Warning]\x1B[0m",
	},
	[_VERBOSITY_INFO] = {
		[0] = "[Info]",
		[1] = "\x1B[1;32m[Info]\x1B[0m",
	},
	[_VERBOSITY_DEBUG] = {
		[0] = "[Debug]",
		[1] = "\x1B[1;35m[Debug]\x1B[0m",
	},
};

void _log(enum _verbosity verbosity, const char *filename, u32 line, const char *format, va_list args) {
	if (verbosity <= _VERBOSITY) {
		fprintf(stderr, "%s[%s:%d] ", verbosity_prefixes[verbosity][isatty(STDERR_FILENO)], filename, line);
		vfprintf(stderr, format, args);
		fprintf(stderr, "\n");
	}
}

void _log_error(const char *filename, u32 line, const char *format, ...) {
	va_list args;
	va_start(args, format);
	_log(_VERBOSITY_ERROR, filename, line, format, args);
	va_end(args);
}

void _log_warning(const char *filename, u32 line, const char *format, ...) {
	va_list args;
	va_start(args, format);
	_log(_VERBOSITY_WARNING, filename, line, format, args);
	va_end(args);
}

void _log_info(const char *filename, u32 line, const char *format, ...) {
	va_list args;
	va_start(args, format);
	_log(_VERBOSITY_INFO, filename, line, format, args);
	va_end(args);
}

void _log_debug(const char *filename, u32 line, const char *format, ...) {
	va_list args;
	va_start(args, format);
	_log(_VERBOSITY_DEBUG, filename, line, format, args);
	va_end(args);
}

void info(const char *format, ...) {
	va_list args;
	va_start(args, format);
	fprintf(stderr, "%s ", verbosity_prefixes[_VERBOSITY_INFO][isatty(STDERR_FILENO)]);
	vfprintf(stderr, format, args);
	va_end(args);
}

void log_command(const char *cmd) {
	if (isatty(STDERR_FILENO)) {
		fprintf(stderr, "%c[%s", 27, cmd);
	}
}

bool log_real(void) {
	return isatty(STDERR_FILENO);
}

