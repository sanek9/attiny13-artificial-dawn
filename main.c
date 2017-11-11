#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//time in min = SLEEP_COUNT * WDTIMER * 255 / 60
//SLEEP_COUNT 1 and WDTIMER_4 = 17 min
//SLEEP_COUNT 2 and WDTIMER_4 = 34 min
//SLEEP_COUNT 3 and WDTIMER_4 = 51 min

//SLEEP_COUNT 1 and WDTIMER_1 = 4.25 min
//SLEEP_COUNT 2 and WDTIMER_1 = 8.5 min
//SLEEP_COUNT 3 and WDTIMER_1 = 12.75 min

//SLEEP_COUNT 1 and WDTIMER_0_5 = 2.3 min
//SLEEP_COUNT 2 and WDTIMER_0_5 = 4.25 min
//SLEEP_COUNT 3 and WDTIMER_0_5 = 6.37 min


#define SLEEP_COUNT 1
//#define WDTIMER_1  //Watch Dog Timer in sec (1 or 0.5 or 4)
//#define WDTIMER_0_5 
#define WDTIMER_4 


#if defined(WDTIMER_1)
	#define WDTIMER 1
	#warning 'Watch Dog Timer in 1 sec'
#elif defined(WDTIMER_0_5)
	#define WDTIMER 0.5
	#warning 'Watch Dog Timer in 0.5s'
#elif defined(WDTIMER_4)
	#define WDTIMER 4
	#warning 'Watch Dog Timer in 4 sec'
#endif


unsigned int k = 0;
unsigned int t = 0;
ISR(WDT_vect) {
	if(t==0){
		if(k>254){
			OCR0A = 255;
		}else{
			OCR0A = (k*k)/255+1;
			k++;
		}
	}

	t++;
	if(t>SLEEP_COUNT){
		t=0;
	}
	
}


int main(void)
{
	OCR0A  = 0;//0x10;                          // initial PWM pulse width
	DDRB   |= (1 << PB0);                   // PWM output on PB2
	TCCR0A = (1 << COM0A1) | (1 << WGM00);  // phase correct PWM mode
    
	TCCR0B = (1 << CS01);   // clock source = CLK/8, start PWM
//	TCCR0B = (1<<CS02) | (1<<CS00);
#if defined(WDTIMER_1)
	// set timer to 1s
	WDTCR |= (0<<WDP3) | (1<<WDP2) | (1<<WDP1) | (0<<WDP0);
#elif defined(WDTIMER_0_5)
	// set timer to 0.5s
	WDTCR |= (1<<WDP2) | (1<<WDP0);
#elif defined(WDTIMER_4)
	// set timer to 4 sec
	WDTCR |= (1<<WDP3);
#endif
	

	WDTCR |= (1<<WDTIE);
	WDTCR |= (0<<WDE);
//	
	sei();
//	set_sleep_mode(SLEEP_MODE_ADC);
	set_sleep_mode(SLEEP_MODE_IDLE);
	while(1){
		sleep_mode();
	};
}
