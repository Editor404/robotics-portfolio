# Window AI Robot: ROS2 기반 지능형 창문 청소 로봇

## 1. 프로젝트 개요
- **프로젝트명**: Window Cleaner Robot System (Window AI Robot)
- **GitHub 저장소**: [https://github.com/Editor404/WindowAIRobot](https://github.com/Editor404/WindowAIRobot)
- **개발 환경**: ROS2 (Jazzy / Humble), Ubuntu 24.04, Raspberry Pi 5, Arduino Uno, Gazebo Classic
- **핵심 목표**: 비전 카메라를 통해 창문 프레임 및 오염 영역을 인식하고, 왜곡 보정 및 Homography 투영 변환을 거쳐 3D 작업 평면의 절대 좌표계로 맵핑한 뒤, 31kHz PWM 흡착 블로워와 모터 주행을 실시간 제어하는 로봇 통합 시스템 구축

---

## 2. 시스템 아키텍처 및 ROS2 노드 구성

```
[ROS2 분산 시스템 데이터 플로우 & 노드 토픽 맵]

  /camera/image_raw (Raspberry Pi Camera V2)
       │
       ▼ [calib_parameters.npz 왜곡 계수 보정]
  dirt_target_node (YOLOv8 seg_best.pt 오염 마스크 검출 + best.pt 코너 검출)
       │
       ▼ [3x3 Homography 투영 변환: (u, v) -> (X_cm, Y_cm)]
  /robot/target_pose (geometry_msgs/Pose2D)
       │
       ▼ [IMU /robot/imu_pose.theta 반영: 글로벌 -> 로봇 중심 회전 변환]
  robot_controller_node (Lawn-mower 경로 플래너 & 모터 이동량 계산)
       │
       ▼ [/arduino/motor_command: dx, dy, clean_flag]
  arduino_imu_node (Serial UART Bridge)
       │
       ├─► Arduino Uno: L298N DC 모터 정/역회전 & PWM 속도 제어
       ├─► MPX 압력센서 ADC 모니터링 ──► 31kHz PWM WS7040 블로워 흡착 안전 제어
       └─► GY-85 (ITG-3205) 자이로 각속도 적분 ──► Yaw각 발행 (Bias -1.35 dps 보정)
```

### 1) `dirt_target_node` (비전 처리 및 공간 좌표계 맵핑)
- **카메라 왜곡 보정 (Undistortion)**: 원시 이미지 `/camera/image_raw`에 체커보드로 추출한 내부 파라미터 $K$ 및 왜곡 계수 $(k_1, k_2, p_1, p_2)$를 적용하여 방사/접선 왜곡 제거.
- **YOLOv8 Segmentation**: 학습된 `seg_best.pt`를 통해 불규칙한 형태의 오염 영역 마스크를 분할하고 무게중심 픽셀 $(u_c, v_c)$ 도출.
- **Homography 투영 변환**: 창문 4개 코너(Top-Left, Top-Right, Bottom-Right, Bottom-Left) 픽셀과 실제 창문 규격(80cm × 160cm) 간의 3x3 투영 행렬 $H$를 계산하여 2D 픽셀을 물리적 메트릭 좌표 `/robot/target_pose`($X_{\text{cm}}, Y_{\text{cm}}$)로 변환.

### 2) `robot_controller_node` (로봇 제어 및 경로 계획)
- 목표 좌표와 현재 IMU Yaw각(`/robot/imu_pose.theta`)을 수신하여 글로벌 좌표계의 이동량을 로봇 중심 로컬 좌표계로 회전 변환.
- 전면 청소 커버리지를 위한 잔디깎기(Lawn-mower) 패턴 경로 생성 및 아두이노 모터 명령 발행.

### 3) `arduino_imu_node` & 하드웨어 펌웨어 (실시간 센서 융합 및 흡착 안전 제어)
- **자이로 센서 바이어스 보정**: GY-85 (ITG-3205)의 정지 상태 드리프트 오프셋($-1.35\text{ dps}$)을 측정 보정하여 누적 적분 Yaw각 계산.
- **31kHz 초고속 PWM 블로워 제어**: 24V BLDC 블로워의 가청 주파수 소음 및 지터를 억제하기 위해 하드웨어 타이머 Fast PWM 모드 사용.
- **Hysteresis 흡착 인터록**: 압력 센서 ADC 값에 히스테리시스 밴드($P_{\text{raw}} \le 885$ 정상 흡착, $P_{\text{raw}} > 900$ 흡착 손실)를 적용하여 흡착 불안정 시 45ms 이내에 블로워 출력을 100%로 강제하고 모터 주행을 즉각 중단.

### 4) Gazebo Classic 3D 시뮬레이션 및 Sim2Real 검증
- `jagosipda.stl` visual mesh를 적용한 30x30cm URDF 로봇 모델링.
- 창문(80x160cm) 및 프레임 World 환경에서 궤적 생성, 가상 오염 모델 삭제 서비스(`gazebo_dirt_cleaner_node`) 및 10개 이상의 pytest 단위 테스트 완료.

---

## 3. Sim2Real 엔지니어링 검증 매트릭스

| 항목 | Gazebo 3D 시뮬레이션 | 실제 물리 하드웨어 (Real Robot) | 비고 |
| :--- | :--- | :--- | :--- |
| **Homography 평면 좌표 오차** | RMSE 0.21 cm | **RMSE 0.94 cm** | $\pm 1.0\text{cm}$ 목표 달성 |
| **흡착 손실 안전 반응 시간** | Contact 센서 즉시 반응 | **45 ms 이내** | 31kHz PWM 강제 출력 & 모터 차단 |
| **자이로 적분 드리프트** | 0.0 dps | **0.02 dps** | 정지 바이어스 보정 적용 |
| **단위 테스트 커버리지** | 10개 pytest 통과 | 노드 간 토픽 통신 무결성 검증 | Pass |

---

## 4. 트러블슈팅 및 핵심 엔지니어링 성과

1. **원근 투영 왜곡으로 인한 픽셀-물리 거리 불일치 해결**:
   - 로봇 장착 카메라의 경사각(Pitch 5.6°, Yaw -10.0°)으로 인해 영상 상단부와 하단부의 픽셀당 실제 거리비가 크게 달랐음.
   - 카메라 캘리브레이션과 $3 \times 3$ Homography 투영 행렬 $s\mathbf{x} = \mathbf{H}\mathbf{X}$를 유도하여 2D 픽셀을 실제 물리 좌표(cm)로 오차 $\pm 1.0\text{cm}$ 이내로 정밀 맵핑.
2. **저가형 IMU 센서의 시간에 따른 누적 적분 발산 해결**:
   - 100회 이상의 정지 상태 데이터 샘플링을 통해 자이로 드리프트 평균값(-1.35 dps)을 도출하고 런타임 동적 파라미터로 상쇄.
3. **압력 센서 노이즈로 인한 급격한 모터 헌팅 및 탈락 방지**:
   - 단순 Threshold 적용 시 발생하는 블로워 온/오프 진동을 방지하기 위해 히스테리시스 밴드 필터를 설계하고, 31kHz PWM 고속 하드웨어 타이머로 밀착 반응성을 45ms 이내로 확보.

---

## 5. 산업용 표준 프레임워크 확장성 (Extensibility)
- **MoveIt2 / Nav2 인터페이스**: 현재 구현된 `/robot/target_pose` 및 TF2 브리지는 향후 ROS2 Nav2(Navigation 2) Costmap 레이어 및 MoveIt2 Cartesian Path Planner와 직접 호환 가능한 표준 메시지 포맷(`geometry_msgs/PoseStamped`, `nav_msgs/OccupancyGrid`)으로 확장이 용이하도록 모듈화 설계되었습니다.
