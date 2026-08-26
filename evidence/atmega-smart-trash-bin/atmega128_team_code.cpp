#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// ---- UART 출력 함수 ----
void uart_init(void) {
	UBRR0H = 0;
	UBRR0L = 103; // 9600bps
	UCSR0B = (1<<TXEN0);
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}
void uart_transmit(char data) {
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
}
void uart_print(const char *str) {
	while(*str) uart_transmit(*str++);
}

// ---- 필터 함수 ----
float IIR_filter(float in, float alpha, float* prev) {
	*prev = alpha * in + (1 - alpha) * (*prev);
	return *prev;
}
#define FIR_N 8
float FIR_filter(float in, float* buf, uint8_t* idx) {
	buf[*idx] = in;
	(*idx)++;
	if (*idx >= FIR_N) *idx = 0;
	float sum = 0;
	for (int i = 0; i < FIR_N; i++) sum += buf[i];
	return sum / FIR_N;
}
float Kalman_filter(float z, float* x, float* p, float q, float r) {
	float xp = *x;
	float pp = *p + q;
	float k = pp / (pp + r);
	*x = xp + k * (z - xp);
	*p = (1 - k) * pp;
	return *x;
}

// ---- DHT11 통신 함수 (PE0, 습도만) ----
#define DHT11_ERROR 255
#define DHT11_DDR DDRE
#define DHT11_PORT PORTE
#define DHT11_PIN PINE
#define DHT11_INPUTPIN PE0
uint8_t getdata(uint8_t select) {
	uint8_t bits[5];
	uint8_t i, j = 0;
	memset(bits, 0, sizeof(bits));
	DHT11_DDR |= (1<<DHT11_INPUTPIN);
	DHT11_PORT |= (1<<DHT11_INPUTPIN);
	_delay_ms(100);

	DHT11_PORT &= ~(1<<DHT11_INPUTPIN);
	_delay_ms(18);
	DHT11_PORT |= (1<<DHT11_INPUTPIN);
	_delay_us(1);
	DHT11_DDR &= ~(1<<DHT11_INPUTPIN);
	_delay_us(39);

	if((DHT11_PIN & (1<<DHT11_INPUTPIN))) return DHT11_ERROR;
	_delay_us(80);
	if(!(DHT11_PIN & (1<<DHT11_INPUTPIN))) return DHT11_ERROR;
	_delay_us(80);

	for (j=0; j<5; j++) {
		uint8_t result=0;
		for(i=0; i<8; i++) {
			while(!(DHT11_PIN & (1<<DHT11_INPUTPIN)));
			_delay_us(30);
			if(DHT11_PIN & (1<<DHT11_INPUTPIN))
			result |= (1<<(7-i));
			while(DHT11_PIN & (1<<DHT11_INPUTPIN));
	''	}
		bits[j] = result;
	}

	DHT11_DDR |= (1<<DHT11_INPUTPIN);
	DHT11_PORT |= (1<<DHT11_INPUTPIN);
	_delay_ms(100);

	if (bits[0] + bits[1] + bits[2] + bits[3] == bits[4]) {
		if (select == 0) return(bits[2]);
		else if(select == 1) return(bits[0]);
	}
	return DHT11_ERROR;
}
uint8_t DHT11_ReadHumidity(void) {
	uint8_t hum = getdata(1);
	if(hum == DHT11_ERROR) return 0;
	return hum;
}

// ---- ADC 초기화 및 읽기 ----
void ADC_init(void) {
	ADMUX = 0x40;
	ADCSRA = 0x87;
}
uint16_t ADC_read(uint8_t ch) {
	ADMUX = (0x40) | (ch & 0x07);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

// ---- PWM/서보/LED 제어 함수 ----
void PWM_init(void) {
	DDRB |= (1<<PB5) | (1<<PB6); // PB5: 서보, PB6: 빨간 LED
	DDRD |= (1<<PD7);            // PD7: 노란 LED
	DDRE |= (1<<PE3);            // PE3: 초록 LED
	TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
	ICR1 = 39999;
	TCCR2 = (1<<WGM20)|(1<<WGM21)|(1<<COM21)|(1<<CS21);
	TCCR3A = (1<<COM3A1)|(1<<WGM31);
	TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31);
	ICR3 = 39999;
}
void set_servo(uint16_t angle) {
	OCR1A = 2000 + (angle * 1000) / 180;
}
void set_red_led(uint8_t val)   { OCR1B = val * 156; }
void set_yellow_led(uint8_t val){ OCR2  = val; }
void set_green_led(uint8_t val) { OCR3A = val * 156; }

// ---- CDS 8개 LED 제어 (PORTC) ----
void cds_leds_update(uint16_t cds_adc) {
	uint8_t led_count = 8 - (cds_adc / 128);
	if (led_count > 8) led_count = 8;
	if (led_count == 0) PORTC = 0x00;
	else PORTC = (1 << led_count) - 1;
}

// ---- 메인 ----
int main(void) {
	uart_init();
	ADC_init();
	PWM_init();
	DDRF &= ~((1<<PF1)|(1<<PF3)|(1<<PF0)); // CDS:PF1, NTC:PF3, POT:PF0 입력
	DDRE &= ~((1<<PE0)|(1<<PE1));          // DHT11:PE0, TCRT5000:PE1 입력
	DDRC = 0xFF; // 8개 LED(단색) 출력

	// 필터 변수 선언
	float cds_iir = 0;
	float ntc_fir_buf[FIR_N] = {0}; uint8_t ntc_fir_idx = 0;
	float dht_kalman = 50, dht_p = 1;
	float tcrt_iir = 0;
	float pot_iir = 0;

	uint8_t target_angle = 0, current_angle = 0;
	uint16_t delay_time = 0;

	char buf[100];

	while (1) {
		// 1. CDS(PF1): IIR(저역통과)
		uint16_t cds_adc = ADC_read(1);
		float cds_filtered = IIR_filter((float)cds_adc, 0.15, &cds_iir);
		cds_leds_update((uint16_t)cds_filtered);

		// 2. NTC(PF3): FIR(이동평균)
		uint16_t ntc_adc = ADC_read(3);
		float ntc_filtered = FIR_filter((float)ntc_adc, ntc_fir_buf, &ntc_fir_idx);
		set_red_led((uint8_t)(ntc_filtered / 4)); // 0~1023 → 0~255

		// 3. DHT11(PE0): 칼만
		uint8_t hum = DHT11_ReadHumidity();
		float hum_filtered = Kalman_filter((float)hum, &dht_kalman, &dht_p, 0.01, 2.0);
		set_green_led((uint8_t)(hum_filtered * 2)); // 0~100 → 0~200

		// 4. TCRT5000(PE1): IIR(저역통과)
		uint16_t tcrt_adc = ADC_read(7); // PF7(ADC7) 예시
		float tcrt_filtered = IIR_filter((float)tcrt_adc, 0.18, &tcrt_iir);
		if (tcrt_filtered >= 500) target_angle = 90;
		else target_angle = 0;

		// 5. 가변저항(PF0): IIR(지수평활)
		uint16_t pot_adc = ADC_read(0);
		float pot_filtered = IIR_filter((float)pot_adc, 0.15, &pot_iir);
		set_yellow_led((uint8_t)(pot_filtered / 4)); // 0~1023 → 0~255
		delay_time = 5 + ((uint16_t)pot_filtered * 45 / 1023);

		// 서보 각도 제어
		if (current_angle < target_angle) current_angle++;
		else if (current_angle > target_angle) current_angle--;
		set_servo(current_angle);

		// ---- 시리얼로 값만 콤마로 구분해 출력 ----
		// 순서: CDS(IIR), NTC(FIR), DHT11(Kalman), TCRT5000(IIR), POT(IIR), 서보각, 빨강LED, 초록LED, 노랑LED
		snprintf(buf, sizeof(buf), "%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
		(uint16_t)cds_filtered,
		(uint16_t)ntc_filtered,
		(uint8_t)hum_filtered,
		(uint16_t)tcrt_filtered,
		(uint16_t)pot_filtered,
		current_angle,
		(uint8_t)(ntc_filtered / 4),           // 빨강 LED PWM
		(uint8_t)(hum_filtered * 2),           // 초록 LED PWM
		(uint8_t)(pot_filtered / 4)            // 노랑 LED PWM
		);
		uart_print(buf);

		// 속도 조절
		for (uint16_t i = 0; i < delay_time; i++) _delay_ms(1);
	}
}
