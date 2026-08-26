/*
 * ExpRobot3.cpp
 *
 * Created: 2025-05-20 오후 12:48:21
 * Author : mario
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define  UBBR 103

volatile int g_cnt = 0;

void USART0_TX(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

unsigned char USART0_RX(void)
{
	while(!(UCSR0A & (1<< RXC0)));
	
	return UDR0;
}

void USART_NUM(int nNum)
{
	USART0_TX((nNum % 10000)/ 1000 + 48); //1000의 자리
	USART0_TX((nNum % 1000)/ 100 + 48); // 100의 자리
	USART0_TX((nNum % 100)/ 10 + 48); // 10의 자리
	USART0_TX((nNum % 10) + 48); //1의 자리
}

double get_cds()
{
	ADMUX = 0x01;
	
	ADCSRA |= (1<<ADSC);
	while(!(ADCSRA & (1<<ADIF)));
	
	int adc = ADC;
	double Vadc = adc * 5.0 / 1023.0;
	
	double R9 = 4700.0;
	double Rcds = (R9 * 5.0 / Vadc)-R9;
	
	double gamma = 0.8;
	double Rb = 40000.0;
	
	double lux = pow(10.0, 1.0-(log10(Rcds) - log10(Rb))/gamma);
	
	return lux;
}

ISR(TIMER2_OVF_vect)
{
	g_cnt ++;
	
	if(g_cnt == 10)
	{
		g_cnt = 0;
		double adc_cds = get_cds();
		
		USART_NUM(adc_cds);
		
		USART0_TX(13);
	}	
	TCNT2 = 255-156;
}

int main(void)
{
	//ADC
	DDRF = 0x00;
	ADMUX = 0x00;
	ADCSRA = 0x87;
	
	//UART
	UBRR0L = (unsigned char)UBBR;
	UBRR0H = (unsigned char)(UBBR>>8);
	
	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	
	unsigned char data;
	
	//Timer2
	TCCR2 = 0x05;
	TCNT2 = 255-156;
	TIMSK = (1<<TOIE2);
	
	sei();
	
	while(1)
	{
		
	}
}