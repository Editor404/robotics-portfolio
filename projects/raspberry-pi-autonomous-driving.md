# Raspberry Pi 5 기반 온디바이스 비전 자율주행 및 FSM 제어

## 1. 프로젝트 개요
- **수행 교과**: 온디바이스 인공지능 (2024년 2학기)
- **개발 환경**: Raspberry Pi 5, Python 3.10, TensorFlow/Keras, OpenCV, YOLOv5n, DC/Servo Motor
- **공개 원본 근거**: [`evidence/on-device-ai/`](../evidence/on-device-ai/) (`self-driving-model-training.ipynb`, `on-device-ai-team-report.docx`, `lane_navigation_*.h5`)

---

## 2. 시스템 아키텍처 및 파이프라인

```
Camera Frame (640x480) ──► ROI Crop (차선 영역 분할)
                              │
  ┌───────────────────────────┴───────────────────────────┐
  ▼ [스레드 1: 차선 추종 고속 제어]                        ▼ [스레드 2: 객체 인식 비동기 큐]
TensorFlow CNN Regression                              YOLOv5n Detector
조향 각도 예측 (-1.0 ~ +1.0)                            표지판/신호등/장애물 Bounding Box
  │                                                       │
  └───────────────────────────┬───────────────────────────┘
                              ▼
                FSM Decision Controller (6 States)
           [DRIVE / STOPPING / RED_WAIT / YELLOW_WAIT / GREEN_STOP / INTERSECTION]
                              │
                              ▼
                   DC/Servo Motor & Buzzer Driver (PWM)
```

### 1) 차선 조향 CNN 회귀 모델 (NVIDIA Architecture)
- 입력: $66 	imes 200 	imes 3$ YUV 색공간 정규화 이미지
- 구조: 5개 Conv2D 레이어 (ELU 활성화, $2	imes 2$ 서브샘플링) + 4개 Dense 레이어
- 손실 함수: MSE (Mean Squared Error), Adam Optimizer
- 출력: 연속형 조향각 ($-1.0 \sim +1.0$)

### 2) YOLOv5n 객체 감지 및 멀티스레딩 최적화
- 라즈베리파이 5 CPU 환경에서 딥러닝 추론 병목을 해결하기 위해 영상 캡처-조향 루프와 YOLOv5n 감지 루프를 독립 스레드로 분리.
- 차선 제어 루프를 25 FPS 이상으로 유지하면서 전방 표지판(정지, 방향) 및 신호등(적/황/녹)을 실시간 검출.

### 3) 유한 상태 머신 (FSM) 의사결정 로직
- `DRIVE`: 정상 차선 추종 주행
- `STOPPING`: 정지선 또는 정지 표지판 감지 시 감속 및 정지
- `RED_WAIT` / `YELLOW_WAIT`: 적색/황색 신호등 감지 시 대기
- `GREEN_STOP`: 녹색 신호 점등 후 일시정지 해제
- `INTERSECTION`: 교차로 표지판 인식에 따른 방향 전환 제어

---

## 3. 핵심 코드 스니펫

```python
# 조향 예측 및 FSM 상태 제어 루프 예시
def control_loop(frame, cnn_model, yolo_results, fsm_state):
    # 1. 차선 ROI 크롭 및 전처리
    roi = preprocess_lane_image(frame[240:480, :])
    steering_angle = float(cnn_model.predict(roi, verbose=0)[0][0])

    # 2. YOLO 감지 결과로부터 이벤트 추출
    detected_objects = parse_yolo_boxes(yolo_results)

    # 3. FSM 상태 전이
    if "traffic_red" in detected_objects:
        fsm_state = "RED_WAIT"
    elif "stop_sign" in detected_objects and fsm_state == "DRIVE":
        fsm_state = "STOPPING"
    elif fsm_state == "RED_WAIT" and "traffic_green" in detected_objects:
        fsm_state = "DRIVE"

    # 4. 상태별 모터 구동값 결정
    speed, steer = execute_fsm_action(fsm_state, steering_angle)
    motor_driver.set_speed(speed)
    motor_driver.set_steering(steer)
    return fsm_state
```
