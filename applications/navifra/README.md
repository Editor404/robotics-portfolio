# 나비프라 물류 로봇 자율주행 S/W 개발자 맞춤 포트폴리오 요약

> 지원 직무: 물류 로봇 자율주행 S/W 개발자  
> 핵심 키워드: 자율주행, C++, Linux, 물류 로봇, 로봇 S/W, SLAM/Mapping, Path Planning, Motion/Control

## 1. 직무 이해

나비프라는 물류 현장의 이동 자동화를 목표로 자율주행 로봇 소프트웨어와 인프라를 개발하는 기업입니다. 공고의 핵심 키워드는 `자율주행`, `C++`, `Linux`이며, 회사 솔루션 관점에서는 LiDAR 기반 자율주행, 멀티로봇 경로계획/관제, 로봇-인프라 통합이 중요합니다.

## 2. 맞춤 핵심 역량

| 나비프라 직무 키워드 | 연결되는 경험 | 근거 |
|---|---|---|
| 물류 로봇 자율주행 S/W | 로봇프로그래밍 프로젝트에서 전체 시스템 설계, 모터제어, SLAM 맵 생성, 전역 Path Planning 기반 자율주행 통합 | `로봇프로그래밍/팀플git.txt`, Project-for-RobotProgramming |
| 자율주행 인지/제어 파이프라인 | Raspberry Pi 5 온디바이스 비전 자율주행: CNN 조향각 회귀, YOLOv5n 객체 인식, 6-state FSM 제어 | `evidence/on-device-ai/` |
| 로봇 통합 시스템 | Window AI Robot: ROS2 노드, Homography 좌표 변환, IMU/압력 센서, 하드웨어 인터록 통합 | WindowAIRobot GitHub |
| C++ / 로봇 알고리즘 구현 | C++/Qt 기반 Canny, Harris, Hough, Otsu 알고리즘 직접 구현 | `evidence/cpp-computer-vision/week7-source/` |
| Linux/임베디드 제어 이해 | ATmega128 센서-모터 제어, Timer PWM, UART, 오실로스코프 검증 | `evidence/atmega-smart-trash-bin/` |

## 3. 지원 메시지 초안

저는 로봇 자율주행을 단순 알고리즘 단위가 아니라, 센서 입력·맵 생성·경로계획·모터제어·실제 주행 검증이 연결된 통합 시스템으로 이해하고 구현해온 지원자입니다. 로봇프로그래밍 프로젝트에서는 전체 시스템 설계와 모터제어, SLAM 맵 생성, 이미 구현된 전역 Path Planning 알고리즘을 실제 자율주행 동작으로 연결하는 역할을 맡았습니다. 또한 Raspberry Pi 기반 온디바이스 자율주행 프로젝트에서는 CNN 조향각 회귀와 YOLOv5n 객체 인식을 FSM 제어와 통합했고, Window AI Robot 프로젝트에서는 ROS2 노드, 비전 좌표 변환, IMU/압력 센서, 하드웨어 제어를 하나의 로봇 시스템으로 연결했습니다.

나비프라가 지향하는 물류 로봇 자율주행 S/W는 현장 환경에서 안정적인 주행과 정확한 위치 추정, 경로계획, 제어 안정성이 중요하다고 이해합니다. 저는 신입으로서 C++/Linux 기반 로봇 소프트웨어 역량을 계속 보완하면서, 먼저 기존 자율주행 모듈의 구조와 현장 이슈를 빠르게 이해하고 재현 가능한 테스트와 개선으로 기여하겠습니다.

## 4. 제출 추천 링크

- 맞춤 웹 포트폴리오: `https://editor404.github.io/robotics-portfolio/navifra.html`
- 전체 GitHub 포트폴리오: `https://github.com/Editor404/robotics-portfolio`
