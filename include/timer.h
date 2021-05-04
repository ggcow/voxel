#ifndef TIMER_H
#define TIMER_H

#include "common.h"

struct timer {
	u64 start_time_us;
};

u64 timer_time_us(void);
u64 timer_time_ms(void);
f64 timer_time_s(void);
void timer_start(struct timer *timer);
bool timer_check_ms(const struct timer *timer, u64 time);

#endif
