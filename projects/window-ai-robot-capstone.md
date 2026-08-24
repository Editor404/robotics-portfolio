# Window AI Robot: ROS2 기반 지능형 창문 청소 로봇

## 1. 프로젝트 개요
- **프로젝트명**: Window Cleaner Robot System (Window AI Robot)
- **GitHub**: [https://github.com/Editor404/WindowAIRobot](https://github.com/Editor404/WindowAIRobot)
- **개발 환경**: ROS2 (Jazzy/Humble), Ubuntu 24.04, Raspberry Pi 5, Arduino Uno, Gazebo Classic
- **목표**: 비전 카메라를 통해 창문과 오염 영역을 인식·로컬라이징하고, 흡착 블로워 및 모터 주행을 제어하여 지능적으로 청소하는 로봇 통합 시스템 구축

---

## 2. 시스템 아키텍처 및 ROS2 노드 구성

### 1) `dirt_target_node` (비전 처리 및 공간 좌표 변환)
- 카메라 원시 영상(`/camera/image_raw`) 수신
- `calib_parameters.npz`를 통한 카메라 왜곡 보정(Undistortion)
- YOLOv8 Segmentation(`seg_best.pt`)으로 오염 중심 픽셀 검출
- 창문 코너 모델(`best.pt`) 또는 OpenCV 컨투어로 4개 꼭짓점(TL, TR, BR, BL) 자동 검출
- 3x3 Homography 투영 변환 행렬을 계산하여 영상 픽셀 좌표를 창문 평면의 절대 미터법 좌표(`/robot/target_pose`, 단위: cm)로 발행

### 2) `robot_controller_node` (로봇 경로 및 모터 명령 생성)
- 목표 좌표(`/robot/target_pose`)와 IMU 자세(`/robot/imu_pose.theta`)를 수신
- 전역 맵 좌표계의 이동량을 로봇 중심 로컬 좌표계로 변환
- 아두이노 구동단이 구독할 모터 명령(`/arduino/motor_command`: dx_cm, dy_cm, 청소 플래그) 발행

### 3) `arduino_imu_node` & 임베디드 펌웨어 (센서 융합 및 흡착 제어)
- GY-85 (ITG-3205) 자이로 센서를 20Hz로 샘플링하여 실시간 각속도 및 적분 Yaw각(`/gyro/data`) 발행
- 정지 상태 평균 자이로 바이어스 보정(`gyro_z_bias_dps = -1.35`)
- A0 압력 센서 ADC 값에 히스테리시스(Hysteresis)를 적용하여 흡착 상태(`adhesion_secure`) 판별
- 31kHz 고주파 PWM을 통해 24V BLDC 흡착 블로워 드라이버 정밀 출력 제어 (흡착 손실 시 최대 출력 및 주행 차단)

### 4) Gazebo Classic 3D 시뮬레이션 및 검증
- `jagosipda.stl` visual mesh 기반 30x30cm URDF 로봇 모델링
- 실제 창문(80x160cm, 돌출 창틀 5cm) 및 로봇 카메라 센서 World 모델링
- Lawn-mower(잔디깎기) 방식의 커버리지 경로 계획 및 시뮬레이션 오염 모델 삭제 서비스(`gazebo_dirt_cleaner_node`) 연동

---

## 3. 핵심 기술 및 해결 과제 (Troubleshooting)

1. **원근 왜곡으로 인한 픽셀-실제 거리 불일치 해결**:
   - 로봇이 유리에 부착되어 경사 각도로 촬영할 때 발생하는 왜곡을 해결하기 위해 카메라 내부 파라미터 캘리브레이션과 Homography 행렬 변환 파이프라인을 구축. 오염 위치의 절대 좌표 오차를 최소화.
2. **저가형 자이로 센서 드리프트 극복**:
   - ITG-3205 센서의 누적 적분 드리프트를 방지하기 위해 정지 상태 오프셋(-1.35 dps)을 캘리브레이션하고, ROS2 launch 파라미터로 동적 주입.
3. **유리 표면 추락 방지 흡착 안전성 확보**:
   - 단순 threshold 대신 Hysteresis 밴드(`ADHESION_TARGET_RAW` vs `ADHESION_RELEASE_RAW`)를 두어 압력 노이즈로 인한 오작동을 방지하고, 31kHz PWM 하드웨어 타이머로 고속 반응성 확보.

---

## 4. 테솔로 로봇 S/W 직무 기여점
- **로봇 통합 시스템**: 비전, 센서, 상위 ROS2 플래너, 하위 MCU 펌웨어를 직접 통합해본 실무 개발 역량
- **비전 카메라 활용**: 2D 픽셀을 물리적 로봇 조작 공간으로 맵핑하는 정밀 비전 좌표계 처리 능력
- **시뮬레이션 & 테스트**: Gazebo 및 실제 하드웨어를 아우르는 검증 및 트러블슈팅 능력
