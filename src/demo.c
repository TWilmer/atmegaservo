/* Name: main.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-09
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 */

/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.
We assume that an LED is connected to port B bit 0. If you connect it to a
different port or bit, change the macros below:
*/

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/io.h>

#include "servo.h"

/* ------------------------------------------------------------------------- */
#define    UCSRA    UCSR0A
#define    UCSRB    UCSR0B
#define    UCSRC    UCSR0C
#define    UBRRH    UBRR0H
#define    UBRRL    UBRR0L
#define    UDRE    UDRE0
#define    UDR    UDR0
#define    RXC    RXC0
#define    RXEN    RXEN0
#define    TXEN    TXEN0
#define    UCSZ1    UCSZ01
#define    UCSZ0    UCSZ00 
#define    U2X    U2X0 
void config_baud115200()
{
    // U2X=1
    UCSRA |= (1 << U2X);
    #undef BAUD  // avoid compiler warning
    #define BAUD 115200
    #include <util/setbaud.h>
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    UCSRB = _BV(TXEN) | _BV(RXEN); /* tx/rx enable */
    UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
}

void putChar(char c)
{
  
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
}

unsigned char getChar( void )
{
/* Wait for data to be received */
while ( !(UCSR0A & (1<<RXC0)) )
;
/* Get and return received data from buffer */
return UDR;
}
    

#include "irmp/irmp.h"
extern void timer1_init();


int __attribute__((noreturn)) main(void)
{
    IRMP_DATA   irmp_data;

    DDRC =1<<0;
    PORTC |=1<<0;

    irmp_init();                                                            // initialize IRMP
    timer1_init();                                                          // initialize timer1

    sei ();                                                                 // enable interrupts
    wdt_enable(WDTO_8S);

 

    DDRD = 1<<5 | 1<<6 ;
    DDRC = 1<<2 | 1<<3 | 1<<4 | 1<<5;
    uint8_t count; 
    for (;;)
    {
       count++;
       if(count>128)
           PORTC|=1;
       else
           PORTC&=~1;
        wdt_reset();
       _delay_ms(10);
    cli ();
    uint8_t ir=irmp_get_data (&irmp_data);
    sei ();                                                                 // enable interrupts
        if (ir)
        {
          if( irmp_data.protocol==7 && 
              irmp_data.address==0x1F
              ){
            if(irmp_data.command==0x20){ //front
              PORTD = 0;
              _delay_ms(10);
	      PORTC = 1<<2 | 1<<4;
              PORTD = 1<<5 | 1<<6;
            }
            if(irmp_data.command==0x21){ //back
              PORTD = 0;
              _delay_ms(10);
	      PORTC = 1<<3 | 1<<5;
              PORTD = 1<<5 | 1<<6;
            }
            if(irmp_data.command==0x11){ //left
              PORTD = 0;
              _delay_ms(10);
	      PORTC = 1<<3 | 1<<4;
              PORTD = 1<<5 | 1<<6;
            }if(irmp_data.command==0x10){ //right
              PORTD = 0;
              _delay_ms(10);
	      PORTC = 1<<2 | 1<<5;
              PORTD = 1<<5 | 1<<6;
            }
            if(irmp_data.command==0x25) {
              PORTD = 0;
            }
          }
        }
    }

   

   ADCSRA =0; // disable ADC to save power
    wdt_enable(WDTO_8S);
    /* Even if you don't use the watchdog, turn it off here. On newer devices,
     * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
     */
    sei();
    DDRB = 1<<1;


        DDRD = 1<<5 | 1<<6 ;
	DDRC = 1<<2 | 1<<3 | 1<<4 | 1<<5;
/*
config_baud115200();
    while(0) {
        if(UCSR0A & (1<<RXC0))
        {
          char c=getChar();
          if(c=='a')
            putChar('Y');
          else
            putChar('N');
        if(c=='8')
	PORTC = 1<<2 | 1<<4;
        if(c=='2')
	PORTC = 1<<3 | 1<<5;
        if(c=='4')
	PORTC = 1<<3 | 1<<4;
        if(c=='6')
	PORTC = 1<<2 | 1<<5;

        if(c=='5')
         PORTD=0;
        else
         PORTD = 1<<5 | 1<<6;

           
        }
        wdt_reset();
        if((PINB & (1<<2))==(1<<2))
        { 
           PORTB &= ~(1<<1);
        } else {
           PORTB |= 1<<1;
        }
    }
*/
    PORTB|=2;
    DDRB=2;
    uint8_t a,b=0;
    int8_t dira=1;
    int8_t dirb=2;
    servo_init();
    for(;;){
        wdt_reset();
    if( a > 2*CENTER )
     dira=-1;
    if(a<5)
     dira=1;
    if( b > 2*CENTER )
      dirb=-2;
    if(b<5)
      dirb=2;
     a=a+dira;
     b=b+dirb;
      servo_set(0,a);
      servo_set(1,2*CENTER);
   _delay_ms( 10 );
    }
    PORTC|=2;
    DDRC=2;
    int v=0;
    int dir=5;
    int i;
    for(;;){                /* main event loop */
	v=dir+v;
        if(v>2000)
        {
          dir=-50;
        }
        if(v<0)
        {
          dir=50;
          v=0;
        }
        wdt_reset();
        PORTC|=2;
_delay_us(700);
        for(i=0;i<v;i++)
        {
          _delay_us(0.2);
        }
        PORTC&=~2;
_delay_ms(20);
        
    }
}

/* ------------------------------------------------------------------------- */
