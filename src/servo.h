#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>

#define PRESCALER      64
#define PRESCALER_BITS (1<<CS22) | ( 1 << CS20 )

#define HALF_MILLISEC   60
#define CENTER         90

extern void servo_init();
extern void servo_set(uint8_t servo, uint8_t value);

#endif
