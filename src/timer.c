#include "timer.h"

#include <time.h>

u64 timer_time_us(void)
{
    struct timespec timespec = {};
	clock_gettime(CLOCK_MONOTONIC, &timespec);
	return ((u64)timespec.tv_sec * 1000000)
		+ ((u64)timespec.tv_nsec / 1000);
}

u64 timer_time_ms(void)
{
	return timer_time_us() / 1000;
}

f64 timer_time_s(void)
{
	return (f64) timer_time_us() / 1000000.0;
}
