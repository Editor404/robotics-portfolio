# ATmega128 기반 센서-모터 제어 시스템 (스마트 쓰레기통)

## 1. 프로젝트 개요
- **수행 교과**: 로봇학실험 3 (2023년 2학기)
- **개발 환경**: ATmega128 (16MHz), Microchip Studio, C/C++, MATLAB, Digital Oscilloscope
- **본인 역할**: **전체 시스템 설계, 하드웨어 회로 구현(100%), MCU 펌웨어 개발, UART 통신 및 오실로스코프 측정**
- **공개 원본 근거**: [`evidence/atmega-smart-trash-bin/`](../evidence/atmega-smart-trash-bin/) (`main.cpp`, `atmega128_team_code.cpp`, 하드웨어 동작 영상)

---

## 2. 하드웨어 회로 및 펌웨어 설계

### 1) 센서 인터페이스
- **TCRT5000 (적외선 반사 센서)**: 물체/손동작 접근 시 아날로그 전압 변화 감지 $
ightarrow$ 인터럽트/ADC 변환
- **Thermistor (NTC 서미스터)**: 온도 변화에 따른 저항/전압 분압 감지 $
ightarrow$ 이상 고온(25°C~35°C 이상) 감지
- **DHT11 (디지털 온습도 센서)**: 1-Wire 시리얼 통신으로 습도 데이터 획득
- **CDS (조도 센서)**: 빛 감지를 통한 쓰레기통 닫힘 상태 판별
- **가변저항 (Potentiometer)**: 사용자가 원하는 서보모터 개폐 속도 조절

### 2) Timer1 Fast PWM 기반 서보모터 정밀 구동
- 16비트 Timer1을 Fast PWM 모드로 설정 (TOP = `ICR1 = 399`, Prescaler = 64).
- 50Hz (20ms 주기) 서보모터 제어 펄스 생성으로 떨림(Jitter) 없는 부드러운 뚜껑 자동 개폐 구현.

### 3) 신호 노이즈 필터링 (IIR & Kalman Filter)
- 아날로그 센서 노이즈 감쇄를 위해 1차 IIR 저주파 통과 필터 및 칼만 필터 설계:
  $$y[n] = \alpha x[n] + (1-\alpha)y[n-1]$$
- UART 직렬 통신으로 MATLAB에 실시간 스트리밍하여 필터 전/후의 신호 대 잡음비(SNR) 비교.

### 4) 오실로스코프 신호 검증
- TCRT5000 전압 변화, DHT11 통신 펄스, 서보모터 PWM $V_{p-p}$ 및 듀티비를 오실로스코프로 실측하여 펌웨어 타이밍 무결성 검증.

---

## 3. 핵심 펌웨어 코드 스니펫

```c
// Timer1 Fast PWM 설정 및 서보모터 제어 (main.cpp 발췌)
void Init_Timer1_PWM(void) {
    // Fast PWM 모드 (Mode 14: ICR1을 TOP으로 사용)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10); // Prescaler 64
    ICR1 = 4999; // 20ms 주기 (50Hz)
    OCR1A = 375; // 서보모터 중립 위치 (1.5ms 펄스폭)
}

void Set_Servo_Angle(int angle) {
    // 0도 ~ 180도에 매핑되는 OCR1A 펄스폭 계산 (1ms ~ 2ms)
    OCR1A = 250 + (angle * 250 / 180);
}
```
