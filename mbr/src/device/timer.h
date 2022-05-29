#ifndef __TIMER_H
#define	__TIMER_H
#include "../lib/kernel/stdint.h"
static void frequency_set(uint8_t counter_port, uint8_t counter_no, uint8_t rwl, uint8_t counter_mode, uint16_t counter_value);
static void intr_timer_handler(void);
void timer_init();
static void ticks_to_sleep(uint32_t sleep_ticks);
void mtime_sleep(uint32_t m_seconds);
#endif
