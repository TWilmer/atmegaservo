
#include <avr/io.h>
#include <avr/interrupt.h>

#include "servo.h"
 
//
// Konfiguration der Servoleitungen
//
#define NR_SERVOS      2
#define SERVO_DDR      DDRC
#define SERVO_PORT     PORTC
uint8_t ServoPuls[NR_SERVOS] = { 1<<PC0, 1<<PC1, };

volatile uint8_t ServoValue[NR_SERVOS];
volatile uint8_t offcount=0;

ISR (TIMER2_COMPA_vect) 
{
  return;
  static uint8_t ServoId = 0;

  if(offcount>0)
  {
     offcount--;
     OCR2A = HALF_MILLISEC ;
  TIFR2= 1<<OCF2A;
     TCNT2=0;
     return ;
  }
 
 
  if( ServoId < NR_SERVOS )
  SERVO_PORT &= ~ServoPuls[ServoId];
  ServoId++;
  if( ServoId == NR_SERVOS ){
    offcount=25;
  OCR2A =  HALF_MILLISEC;
  TIFR2= 1<<OCF2A;
  TCNT2=0;
   return;
  }
  if(ServoId > NR_SERVOS)
  {
  ServoId=0;
  }
 
  if(ServoId<NR_SERVOS){
     SERVO_PORT |= ServoPuls[ServoId];
  offcount=1;
  }
 
  if(ServoId<NR_SERVOS)
  OCR2A =  ServoValue[ServoId];
  else
  OCR2A =  HALF_MILLISEC;
  TIFR2= 1<<OCF2A;
  TCNT2=0;
}

void servo_init()
{
  SERVO_DDR = ServoPuls[0] | ServoPuls[1] ;
 SERVO_PORT=0;
  uint8_t i;
  for( i = 0; i < NR_SERVOS; ++i )
    ServoValue[i] = HALF_MILLISEC;

  OCR2A = ServoValue[0];
  TIMSK2 |= (1<<OCIE2A);
  TCCR2A = (1<<WGM21) ;
  TCCR2B = PRESCALER_BITS;  // CTC mode
}
void servo_set(uint8_t servo, uint8_t value)
{
   ServoValue[servo]=value;
}
