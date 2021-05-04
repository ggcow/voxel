#include "../include/timer.h"

#include <time.h>

u64
timer_time_us(void)
{
	struct timespec timespec = { 0 };
	clock_gettime(CLOCK_MONOTONIC, &timespec);
	return ((u64)timespec.tv_sec * 1000000)
		+ ((u64)timespec.tv_nsec / 1000);
}

u64
timer_time_ms(void)
{
	return timer_time_us() / 1000;
}

f64
timer_time_s(void)
{
	return timer_time_us() / 1000000.0;
}

void
timer_start(struct timer *timer)
{
	timer->start_time_us = timer_time_us();
}

bool
timer_check_ms(const struct timer *timer, u64 time)
{
	return timer->start_time_us == 0 ? TRUE :
		(timer_time_us() - timer->start_time_us) / 1000 > time;
}
