#ifndef LOG_H
#define LOG_H

#include "core/common.h"

void _log_error(const char *filename, u32 line, const char *format, ...);
void _log_warning(const char *filename, u32 line, const char *format, ...);
void _log_info(const char *filename, u32 line, const char *format, ...);
void _log_debug(const char *filename, u32 line, const char *format, ...);

void info(const char *format, ...);
void log_command(const char *cmd);

#define log_error(...) \
	_log_error(__FILE__, __LINE__, __VA_ARGS__)
#define log_warning(...) \
	_log_warning(__FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) \
	_log_info(__FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) \
	_log_debug(__FILE__, __LINE__, __VA_ARGS__)

#endif
