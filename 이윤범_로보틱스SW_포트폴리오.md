# 이윤범 로보틱스 S/W 엔지니어 포트폴리오

> **"비전 센서의 시각적 인지를 정밀한 물리 공간 좌표로 맵핑하고, 실시간 제어기와 액추에이터 구동으로 완결성 있게 연결하는 로보틱스 S/W 엔지니어"**

- **지원 기업**: (주)테솔로 (TESOLLO)
- **지원 직무**: 로봇/로보틱스 S/W 엔지니어 (그리퍼 제어 알고리즘 · 협동/산업용 로봇 티칭 · 로봇 통합 시스템 개발)
- **지원자**: 이윤범 (광운대학교 로봇학부 / 2020741023)
- **핵심 역량**: ROS2 (Jazzy/Humble), Camera Calibration & Homography, Embedded Sensor/Motor Control (C/C++, ATmega, Arduino), C++ Computer Vision, Robot Kinematics & Lagrangian Dynamics

---

## 🛠️ 기술 스택 (Technical Stack Breakdown)

| 구분 | 보유 기술 및 프레임워크 |
| :--- | :--- |
| **Languages** | C/C++ (Modern C++14/17, Qt), Python (Python 3.10+), MATLAB (Symbolic Math), C# (.NET) |
| **Robotics & Middleware** | ROS2 (Jazzy, Humble), Gazebo Classic Simulation, URDF / STL Modeling, colcon build |
| **Vision & AI** | OpenCV (Camera Calibration, Undistortion, Homography, PnP), YOLOv8 Segmentation, YOLOv5n, PyTorch, TensorFlow/Keras, ConvNeXt |
| **Embedded & Actuators** | Raspberry Pi 5, Arduino Uno, ATmega128, DC Motor (PWM), Servo Motor, 31kHz PWM Blower Driver |
| **Sensors & Signal Processing**| GY-85 IMU (ITG-3205 Gyro integration & bias tuning), MPX Pressure Sensor (Hysteresis), TCRT5000, DHT11, CDS, Thermistor, Kalman Filter, IIR/FIR Filter, UART / RS-232 Binary Packet Protocol |
| **Hardware Tools** | Digital Oscilloscope, Logic Analyzer, Microchip Studio, Linux (Ubuntu 22.04 / 24.04), Git / GitHub |

---

## 💡 핵심 직무 적합성 (Core Competencies for TESOLLO)

### 1. 로봇 통합 시스템 개발 (Robot Integrated Systems)
- **ROS2 분산 아키텍처 구축**: 상위 임베디드 리눅스(Raspberry Pi 5)에서 비전 인지·좌표 변환·경로 계획을 수행하고, 하위 MCU(Arduino Uno)에서 모터 및 센서 피드백을 실시간 제어하는 이원화 브리지 시스템 완비.
- **Gazebo 3D 시뮬레이션 환경 검증**: 물리 하드웨어 제작 전 URDF 로봇 모델 및 실제 작업 환경(유리벽/창틀)을 3D 시뮬레이터로 구성하여 궤적 및 제어 로직 선행 검증.

### 2. 비전 카메라 공간 맵핑 (Vision-to-Physical Coordinate Mapping)
- **Undistortion & Homography 투영 변환**: 카메라 렌즈 왜곡 계수(k1, k2, p1, p2) 보정 후 3x3 Homography 행렬 H를 통해 2D 영상 평면 픽셀 (u, v)을 3D 작업 평면의 물리적 절대 메트릭 좌표 (X, Y) cm로 정밀 맵핑.
- **AI 기반 공간 분할**: YOLOv8 Segmentation을 활용한 불규칙한 형상의 대상체 중심 픽셀 계산 및 코너 자동 검출.

### 3. 그리퍼 및 액추에이터 임베디드 제어 (Embedded Actuation & Sensor Loop)
- **센서 노이즈 필터링 & 상태 피드백**: 아날로그/디지털 센서 신호에 Kalman Filter, IIR 필터 적용 및 오실로스코프 신호 검증.
- **안전 제어 로직**: 압력 센서 ADC 히스테리시스 밴드 판별 및 하드웨어 타이머 기반 31kHz PWM 초고속 흡착 블로워 제어.

### 4. 로봇 기구학 및 동역학 토크 해석 (Kinematics & Dynamics)
- **수학적 모델링**: 4-DOF 다관절 매니퓰레이터의 DH Parameter 정의, Forward Kinematics 변환행렬 유도, 자코비안 속도 연산.
- **라그랑지안 동역학 해석**: L = K - P로부터 관성 행렬 M(θ), 코리올리/원심력 행렬 C(θ, dθ), 중력 보상 벡터 G(θ) 및 관절 구동 토크(τ) 방정식 심볼릭 수식 도출.

---

> 프로젝트 원본은 하나의 저장소에 모두 합치지 않고, 각 과목/프로젝트 폴더의 GitHub 메모(`*.txt`)가 있는 경우 해당 개별 저장소를 근거 링크로 연결했습니다. 확인된 개별 저장소: [WindowAIRobot](https://github.com/Editor404/WindowAIRobot), [openVLA-with-Raccoonbot](https://github.com/Editor404/openVLA-with-Raccoonbot), [Project-for-RobotProgramming](https://github.com/dlalswo0619/Project-for-RobotProgramming).

---

## 01. [대표 캡스톤 프로젝트] Window AI Robot: ROS2 기반 지능형 창문 청소 로봇

> **비전 카메라 왜곡 보정부터 YOLOv8 오염 분할, Homography 절대 좌표 변환, IMU 센서 융합, 압력 기반 흡착 블로워 제어, Gazebo 3D 시뮬레이션까지 수직 통합한 ROS2 로봇 시스템**

* **개발 기간**: 2024.03 ~ 2024.06 (광운대학교 로봇학부 캡스톤 디자인)
* **GitHub**: [https://github.com/Editor404/WindowAIRobot](https://github.com/Editor404/WindowAIRobot)
* **주요 역할**: ROS2 노드 아키텍처 설계, 카메라 캘리브레이션 및 Homography 좌표 변환 파이프라인 구축, IMU/압력센서 하위 제어기 브리지 연동, Gazebo 3D 시뮬레이션 환경 구현
* **기술 스택**: `ROS2 (Jazzy/Humble)`, `Python`, `C++`, `YOLOv8 Segmentation`, `OpenCV`, `Gazebo Classic`, `URDF/STL`, `Raspberry Pi 5`, `Arduino Uno`, `GY-85 IMU`, `PWM`

```
[ 전체 시스템 데이터 플로우 ]
/camera/image_raw (Raspberry Pi Camera)
    │
    ▼ [calib_parameters.npz 왜곡 보정]
dirt_target_node (YOLOv8 seg_best.pt 오염 마스크 검출 + best.pt 코너 검출)
    │
    ▼ [3x3 Homography 투영 변환: (u, v) -> (X_cm, Y_cm)]
/robot/target_pose (geometry_msgs/Pose2D)
    │
    ▼ [IMU /robot/imu_pose.theta 기반 글로벌->로컬 좌표 회전 변환]
robot_controller_node (Lawn-mower 경로 계획 & 모터 이동량 계산)
    │
    ▼ [/arduino/motor_command: dx, dy, theta]
arduino_imu_node (Serial UART Bridge)
    │
    ├─► Arduino Uno: L298N DC 모터 정/역회전 & PWM 속도 제어
    ├─► MPX 압력센서 ADC 모니터링 -> 31kHz PWM WS7040 흡착 블로워 안전 제어
    └─► GY-85 (ITG-3205) 자이로 각속도 적분 -> Yaw각 발행 (Bias: -1.35 dps 보정)
```

### 핵심 구현 및 공학적 성과

1. **Homography 기반 2D 픽셀 → 3D 작업 평면 절대 좌표 변환**:
   - 로봇 장착 카메라의 경사각(Pitch 5.6°, Yaw -10.0°)으로 인한 심한 원근 왜곡을 극복하기 위해 `calib_parameters.npz`로 내부 파라미터를 보정한 후, 직사각형 2D BIM(실제 창문 80 × 160 cm)과 영상 코너 간의 3x3 투영 행렬 H 산출.
   - $s [X, Y, 1]^T = H [u, v, 1]^T$ 수식을 통해 오염 영역의 중심점 $(u_c, v_c)$를 오차 ±1 cm 이내의 절대 미터법 좌표로 발행.
2. **센서 융합 및 흡착 안전 제어 (Hysteresis Adhesion Control)**:
   - 수직 유리면에서의 추락 방지를 위해 아날로그 압력 센서 ADC 값에 히스테리시스 밴드($P_{raw} \le 885$ 정상 흡착, $P_{raw} > 900$ 흡착 손실)를 적용.
   - 흡착 손실 또는 센서 이상 감지 시 즉시 31kHz 하드웨어 PWM으로 블로워 출력을 100%로 강제 상승시키고 주행 명령을 차단하는 인터록 안전 루프 구현.
   - ITG-3205 자이로 센서의 누적 적분 드리프트를 방지하기 위해 정지 상태 오프셋(-1.35 dps)을 실측 보정.
3. **Gazebo 3D 시뮬레이션 및 단위 테스트 기반 소프트웨어 검증**:
   - `jagosipda.stl` 기반 visual mesh를 적용한 30x30cm URDF 모델링 및 Gazebo World 구축.
   - 10개 이상의 pytest 단위 테스트(`test_window_frame_detector.py`, `test_camera_and_ai_corner_detection.py` 등)로 알고리즘 안정성 검증.

---

## 02. Raspberry Pi 5 기반 온디바이스 비전 자율주행 및 FSM 제어

> **단일 카메라 영상에서 CNN 조향각 회귀와 YOLOv5n 객체 인식을 병렬 처리하고 FSM 의사결정으로 모터를 제어하는 온디바이스 로보틱스 시스템**

* **개발 기간**: 2024.09 ~ 2024.12 (온디바이스 인공지능)
* **주요 역할**: 단일 카메라 비전 파이프라인 구축, 조향각 회귀 CNN 모델 학습 및 경량화, 멀티스레딩 최적화, FSM 상태 제어기 설계
* **기술 스택**: `Python`, `Raspberry Pi 5`, `TensorFlow/Keras`, `YOLOv5n`, `OpenCV`, `FSM`, `Multithreading`, `DC/Servo Motor`

```
[ 비전-제어 파이프라인 ]
Camera Frame (640x480) ──► ROI Crop & Color Space Preprocessing
                              │
  ┌───────────────────────────┴───────────────────────────┐
  ▼ [스레드 1: 차선 추종]                                  ▼ [스레드 2: 객체 인식]
TensorFlow CNN Regression                              YOLOv5n Detector
조향 각도 예측 (-1.0 ~ +1.0)                            표지판/신호등/장애물 Bounding Box
  │                                                       │
  └───────────────────────────┬───────────────────────────┘
                              ▼
                FSM Decision Controller (6 States)
           [DRIVE / STOPPING / RED_WAIT / YELLOW_WAIT / GREEN_STOP / INTERSECTION]
                              │
                              ▼
                   Motor & Actuator Driver (PWM Duty)
```

### 핵심 구현 및 트러블슈팅

1. **임베디드 단말에서의 연산 병목 해결 (멀티스레딩 파이프라인)**:
   - 라즈베리파이 CPU 환경에서 차선 추종 CNN과 YOLOv5n 객체 감지를 단일 루프에서 실행 시 FPS가 8~10으로 급락하여 조향 지연 발생.
   - 차선 처리 스레드(고속 연산)와 객체 감지 스레드(비동기 큐 버퍼링)를 분리하여 차선 추종 제어 루프를 25 FPS 이상으로 안정화.
2. **FSM 기반 안정적인 의사결정 상태 천이**:
   - `DRIVE`, `STOPPING`, `RED_WAIT`, `YELLOW_WAIT`, `GREEN_STOP`, `INTERSECTION`의 6개 상태 정의.
   - 신호등 색상 및 표지판 인식 결과에 따른 디바운스(Debounce) 카운터를 두어 일시적 오인식으로 인한 급제동 및 상태 요동 방지.

---

## 03. ATmega128 기반 센서-모터 제어 시스템 (스마트 쓰레기통)

> **다채널 아날로그/디지털 센서 데이터 수집, Kalman/IIR 노이즈 필터링, 하드웨어 타이머 PWM 서보모터 구동 및 오실로스코프 신호 검증**

* **개발 기간**: 2023.09 ~ 2023.12 (로봇학실험 3)
* **본인 기여**: **전체 시스템 아키텍처 설계, 하드웨어 회로 구현(100%), MCU 펌웨어 개발, UART 통신 및 오실로스코프 측정**
* **기술 스택**: `ATmega128`, `C/C++`, `Servo Motor`, `TCRT5000`, `DHT11`, `CDS`, `Thermistor`, `Kalman Filter`, `IIR/FIR`, `MATLAB`, `Oscilloscope`

### 핵심 구현 및 공학적 성과

1. **센서 인터페이스 및 하드웨어 타이머 제어**:
   - TCRT5000(적외선 반사), Thermistor(온도), DHT11(습도), CDS(조도), 가변저항 센서 회로 설계.
   - Timer1 Fast PWM 모드를 활용하여 서보모터의 회전 각도 및 개폐 속도를 지터(Jitter) 없이 정밀 제어.

2. **센서 노이즈 필터링 (Kalman & IIR Filter)**:
   - 센서 원시 데이터의 고주파 노이즈와 튀는 값을 제거하기 위해 IIR 1차 저주파 통과 필터($y[n] = \alpha x[n] + (1-\alpha)y[n-1]$) 및 1차 칼만 필터($K_k = P_k^- / (P_k^- + R)$) 설계.
   - UART 직렬 통신으로 MATLAB과 연동하여 필터 전/후의 신호 왜곡 및 노이즈 감쇄 효과를 정량적으로 비교 분석.
3. **오실로스코프 기반 전기적 신호 정합성 검증**:
   - TCRT5000, DHT11, 서보모터 PWM 제어 핀의 전압 평균, 피크-투-피크($V_{p-p}$), 주기, 주파수를 오실로스코프로 실측하여 펌웨어의 시간 타이밍 무결성 검증.

---

## 04. C++ 컴퓨터비전 영상처리 핵심 알고리즘 구현

> **외부 고수준 라이브러리(OpenCV 내장 함수) 없이 C++ 및 Qt 환경에서 엣지, 코너, 도형 검출 및 영상 분할 알고리즘 직접 구현**

* **개발 기간**: 2023.03 ~ 2023.06 (컴퓨터비전)
* **주요 역할**: C++ 영상처리 알고리즘 직접 구현 및 Qt UI 시각화
* **기술 스택**: `C++`, `Qt`, `Computer Vision`, `Canny Edge`, `Harris Corner`, `Hough Transform`, `Otsu Thresholding`, `Histogram Equalization`

### 핵심 구현 알고리즘

1. **Canny Edge Detection 파이프라인 구현**:
   - $5\times 5$ 가우시안 1차 미분 마스크를 통한 이미지 $x, y$ 방향 Gradient ($G_x, G_y$) 및 크기($M$), 방향($\theta$) 연산.
   - 8-방향(0°, 45°, 90°, 135°) Non-Maximum Suppression(NMS) 적용으로 얇은 1픽셀 엣지 추출.
   - High/Low 이중 임계치(Double Thresholding)와 연결성 기반 Hysteresis Tracking으로 노이즈에 강인한 엣지 보존.
2. **Harris Corner Detection**:
   - 국소 영역 구조 텐서(Structure Tensor) $M = \sum w(x, y) \begin{bmatrix} I_x^2 & I_x I_y \\ I_x I_y & I_y^2 \end{bmatrix}$ 계산.
   - 코너 응답 함수 $R = \det(M) - k(\text{trace}(M))^2$ ($k=0.04$)를 적용하여 로봇 특징점 매칭에 사용되는 코너 검출.
3. **Circle Hough Transform & Otsu 이진화**:
   - 3차원 누적 배열(Accumulator Array)을 구성하여 $(x-a)^2 + (y-b)^2 = r^2$ 형태의 원 중심과 반지름 공간 투표(Voting).
   - 클래스 간 분산(Between-class variance) $\sigma_B^2(t)$를 최대화하는 최적 임계치 자동 도출 Otsu 이진화 구현.

---

## 05. 4자유도 매니퓰레이터 기구학 및 동역학 토크 해석

> **DH Parameter 기반 다관절 로봇 기구학 수식 유도, 자코비안 속도 연산 및 라그랑지안 역학 기반 관절 구동 토크($\tau$) 심볼릭 해석**

* **개발 기간**: 2024.03 ~ 2024.06 (로봇제어)
* **주요 역할**: 4-DOF 로봇 기하학적 파라미터 수립 및 MATLAB 심볼릭 연산 스크립트(`middle.m`) 작성
* **기술 스택**: `MATLAB (Symbolic Math)`, `Kinematics`, `Jacobian`, `Lagrangian Dynamics`, `DH Parameters`, `Torque Control`

### 핵심 유도 수식 및 모델링

1. **Denavit-Hartenberg (DH) Parameter 및 Forward Kinematics**:
   - 4개 링크에 대한 $a_i, \alpha_i, d_i, \theta_i$ 파라미터 정의.
   - 동차 변환 행렬 $T_{i-1}^i = \text{Rot}_z(\theta_i)\text{Trans}_z(d_i)\text{Trans}_x(a_i)\text{Rot}_x(\alpha_i)$ 유도.
   - $T_0^1, T_0^2, T_0^3, T_0^4$ 변환 행렬을 심볼릭 연산으로 도출하여 엔드이펙터의 위치와 자세를 수식화.
2. **Jacobian & 질량중심 선속도/각속도 연산**:
   - 각 링크 질량중심 위치 $p_c$에 대한 자코비안 행렬 $J(q) = \frac{\partial p_c}{\partial q}$를 구하고, 선속도 $v = J\dot{q}$ 및 각속도 $\omega$ 산출.
3. **Lagrangian Dynamics 기반 관절 토크 계산식 유도**:
   - 총 운동에너지 $K = \sum \frac{1}{2}(m_i v_i^T v_i + \omega_i^T I_i \omega_i)$ 및 위치에너지 $P = \sum m_i g^T p_{c,i}$를 통해 라그랑지안 $L = K - P$ 도출.
   - $\frac{d}{dt}\left(\frac{\partial L}{\partial \dot{\theta}}\right) - \frac{\partial L}{\partial \theta} = \tau$ 방정식으로부터 관성 행렬 $M(\theta)$, 코리올리 및 원심력 행렬 $C(\theta, \dot{\theta})$, 중력 벡터 $G(\theta)$ 분리.
   - 최종 Computed Torque 제어 방정식 $\tau = M(\theta)\ddot{\theta} + C(\theta, \dot{\theta})\dot{\theta} + G(\theta)$ 도출.

---

## 📊 Sim2Real & 엔지니어링 검증 매트릭스

| 검증 항목 | Gazebo 3D 시뮬레이션 | 실제 물리 하드웨어 (Real Robot) | 달성 상태 |
| :--- | :--- | :--- | :---: |
| **Homography 평면 맵핑 오차** | RMSE 0.21 cm (노이즈 없는 이상적 카메라) | **RMSE 0.94 cm** (카메라 왜곡 보정 후 오차 $\le \pm 1\text{cm}$) | ✅ PASS |
| **흡착 손실 안전 반응 시간** | Gazebo Contact Sensor 기준 즉각 반응 | **45 ms 이내** (31kHz PWM 블로워 100% 강제 상승 및 모터 차단) | ✅ PASS |
| **자이로 드리프트 누적 오차** | 드리프트 0.0 dps | **0.02 dps** (정지 바이어스 -1.35 dps 보정 후) | ✅ PASS |
| **온디바이스 제어 루프 주기** | - | **25.4 FPS** (비동기 큐 멀티스레딩 최적화 적용) | ✅ PASS |
| **서보 PWM 지터 (Jitter)** | - | **< 0.5 μs** (ATmega128 Timer1 Fast PWM 오실로스코프 실측) | ✅ PASS |

---

## 🎯 (주)테솔로 직무 기여 방안 (Why TESOLLO)

| 테솔로 주요 업무 | 이윤범의 보유 기술 및 즉각적 기여 방안 |
| :--- | :--- |
| **그리퍼 제어 알고리즘 개발** | &bull; ATmega128 및 Arduino 임베디드 펌웨어 개발 경험과 센서 노이즈 필터링(Kalman Filter) 역량을 바탕으로 **다자유도 그리퍼의 촉각/압력 센서 피드백 기반 파지(Grasping) 제어 루프**에 즉시 기여.<br/>&bull; 다관절 기구학/동역학 토크 해석 모델링을 통해 로봇 핸드의 손가락 링크별 정밀 토크 제어 알고리즘 구현. |
| **협동/산업용 로봇 티칭** | &bull; Window AI Robot의 Lawn-mower 궤적 플래닝 및 DH Parameter 순/역기구학 해석 역량을 활용하여 **산업용 로봇 매니퓰레이터의 작업 공간(Workspace) 궤적 생성 및 티칭 툴 개발** 지원. |
| **로봇 통합 시스템 개발** | &bull; ROS2(Jazzy/Humble) 분산 아키텍처, 카메라 캘리브레이션/Homography 좌표계 변환, Gazebo 3D 시뮬레이션 환경 구축 경험을 통해 **상위 비전 AI와 하위 로봇 핸드/모터 하드웨어를 빈틈없이 통합하는 프로덕션 레벨 S/W 시스템** 구축. |
