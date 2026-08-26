# 🤖 이윤범 로보틱스 S/W 엔지니어 포트폴리오

<div align="left">
  <img src="https://img.shields.io/badge/ROS2-Jazzy%20%2F%20Humble-blue?logo=ros&logoColor=white" />
  <img src="https://img.shields.io/badge/C%2B%2B-17%20%2F%20Qt-00599C?logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Python-3.10+-3776AB?logo=python&logoColor=white" />
  <img src="https://img.shields.io/badge/Computer_Vision-OpenCV%20%2F%20YOLOv8-5C3EE8?logo=opencv&logoColor=white" />
  <img src="https://img.shields.io/badge/Simulation-Gazebo_Classic-orange" />
  <img src="https://img.shields.io/badge/Embedded-ATmega128%20%2F%20Arduino-00979D?logo=arduino&logoColor=white" />
</div>

> **"비전 카메라의 시각적 인지를 정밀한 물리 공간 좌표로 맵핑하고, ROS2 분산 미들웨어와 임베디드 실시간 제어기로 완결성 있게 연결하는 로보틱스 S/W 엔지니어"**

---

## 🌐 웹 포트폴리오 (Interactive Web Portfolio)
브라우저에서 바로 확인 가능한 인터랙티브 웹 포트폴리오입니다:
- **로컬 실행**: [`index.html`](./index.html) (브라우저로 열람)
- **주요 기능**:
  - 📐 **KaTeX 수식 렌더링**: Homography 변환식, 라그랑지안 동역학 토크식, 칼만 필터 수식
  - 🎛️ **카테고리별 프로젝트 필터링**: ROS2, Vision/AI, Embedded, Dynamics
  - 📊 **Sim2Real 검증 매트릭스**: Gazebo 시뮬레이션 vs 실제 물리 하드웨어 계측 오차 비교
  - 🖨️ **인쇄(PDF) 최적화**: `Ctrl+P` 브라우저 인쇄 시 깔끔한 화이트 테마 문서로 즉시 출력

---

## 🏆 선별 5대 프로젝트 (Key Verified Projects)

| No | 프로젝트명 | 핵심 기술 스택 | 주요 성과 및 역할 |
| :---: | :--- | :--- | :--- |
| **01** | **[Window AI Robot (캡스톤 디자인)](./projects/window-ai-robot-capstone.md)** | `ROS2`, `Python`, `C++`, `YOLOv8`, `Homography`, `Gazebo`, `31kHz PWM` | **대표 프로젝트** · 비전 왜곡 보정, Homography 절대 좌표 변환(오차 $\le \pm 1\text{cm}$), IMU/압력 센서 융합 및 31kHz PWM 흡착 블로워 안전 인터록 제어 ([GitHub 저장소](https://github.com/Editor404/WindowAIRobot)) |
| **02** | **[온디바이스 비전 자율주행](./projects/raspberry-pi-autonomous-driving.md)** | `Raspberry Pi 5`, `TensorFlow`, `YOLOv5n`, `FSM`, `Multithreading` | CNN 조향각 회귀 + YOLOv5n 객체 감지 비동기 스레딩 최적화로 **25+ FPS 달성** 및 6개 상태 FSM 제어 |
| **03** | **[ATmega128 센서-모터 제어기](./projects/atmega-smart-trash-bin.md)** | `ATmega128`, `C/C++`, `Kalman Filter`, `Timer1 PWM`, `Oscilloscope` | **본인 기여 100%** · TCRT5000/온습도 센서 회로 설계, 칼만 필터 노이즈 제거, Timer1 Fast PWM 지터 없는 서보 구동 및 오실로스코프 신호 검증 |
| **04** | **[C++ 컴퓨터비전 알고리즘 구현](./projects/cpp-computer-vision.md)** | `Modern C++`, `Qt`, `Canny Edge`, `Harris Corner`, `Hough Circle` | 외부 라이브러리 없이 순수 C++로 NMS/Hysteresis Canny Edge, 텐서 기반 Harris Corner, Hough 원 검출 직접 코딩 |
| **05** | **[4-DOF 로봇 기구학 및 동역학 해석](./projects/robot-kinematics-dynamics.md)** | `MATLAB Symbolic Math`, `DH Parameters`, `Lagrangian Dynamics` | 4축 매니퓰레이터 DH 변환 행렬, 자코비안, 라그랑지안 $L=K-P$로부터 $\tau = M\ddot{\theta} + C\dot{\theta} + G$ 심볼릭 토크 방정식 도출 |

---

## 📊 Sim2Real & 엔지니어링 검증 매트릭스

| 검증 항목 | Gazebo 3D 시뮬레이션 | 실제 물리 하드웨어 (Real Robot) | 검증 상태 |
| :--- | :--- | :--- | :---: |
| **Homography 평면 좌표 오차** | RMSE 0.21 cm | **RMSE 0.94 cm** (왜곡 보정 후 $\le \pm 1\text{cm}$) | ✅ PASS |
| **흡착 손실 안전 반응 시간** | Gazebo Contact 기준 즉각 반응 | **45 ms 이내** (31kHz PWM 100% 강제 출력) | ✅ PASS |
| **자이로 센서 드리프트** | 0.0 dps | **0.02 dps** (정지 바이어스 -1.35 dps 보정) | ✅ PASS |
| **온디바이스 제어 루프 주기** | - | **25.4 FPS** (비동기 큐 멀티스레딩 최적화) | ✅ PASS |
| **서보모터 PWM 지터 (Jitter)** | - | **< 0.5 μs** (오실로스코프 파형 실측) | ✅ PASS |

---


## 🔗 프로젝트별 GitHub 근거 연결 원칙

이 포트폴리오는 하나의 저장소에 모든 프로젝트 원본을 합쳐 올리기보다, 각 프로젝트 폴더 안의 GitHub 메모(`*.txt`)가 확인되는 경우 해당 프로젝트의 개별 저장소를 1차 근거로 연결합니다.

| 로컬 폴더 | GitHub 근거 저장소 | 비고 |
| :--- | :--- | :--- |
| `캡스톤/` | [Editor404/WindowAIRobot](https://github.com/Editor404/WindowAIRobot) | Window AI Robot 캡스톤 코드 근거 |
| `피지컬ai/` | [Editor404/openVLA-with-Raccoonbot](https://github.com/Editor404/openVLA-with-Raccoonbot) | OpenVLA/Raccoonbot 실험 근거 |
| `로봇프로그래밍/` | [dlalswo0619/Project-for-RobotProgramming](https://github.com/dlalswo0619/Project-for-RobotProgramming) | 로봇프로그래밍 팀 프로젝트 근거 |

---

## 📂 저장소 파일 구조

```
portfolio/
├── index.html                           # 인터랙티브 반응형 웹 포트폴리오
├── README.md                            # 포트폴리오 요약 및 깃허브 메인 문서
├── 이윤범_로보틱스SW_포트폴리오.md       # 서류 제출용 상세 포트폴리오 문서
├── 이윤범_로보틱스SW_포트폴리오.docx     # Word 제출용 문서
├── 테솔로_자기소개서_2020741023_이윤범.md # 맞춤형 자기소개서
├── projects/                            # 세부 프로젝트별 기술 심층 분석 보고서
│   ├── window-ai-robot-capstone.md      # 01. Window AI Robot (ROS2 캡스톤)
│   ├── raspberry-pi-autonomous-driving.md # 02. 온디바이스 비전 자율주행
│   ├── atmega-smart-trash-bin.md        # 03. ATmega128 센서-모터 제어
│   ├── cpp-computer-vision.md           # 04. C++ 영상처리 알고리즘
│   └── robot-kinematics-dynamics.md     # 05. 4-DOF 기구학 및 동역학 해석
└── candidate-review.md                  # 포트폴리오 강점 및 직무 정합성 분석
```
