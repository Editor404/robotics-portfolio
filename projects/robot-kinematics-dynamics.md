# 4자유도 매니퓰레이터 기구학 및 동역학 토크 해석

## 1. 프로젝트 요약
4축 다자유도 로봇 매니퓰레이터의 기하학적 구조를 수학적으로 정의하고, 자코비안 및 라그랑지안 역학을 기반으로 관성, 코리올리, 중력 보상 및 관절 구동 토크 방정식을 유도한 시뮬레이션 프로젝트입니다.

## 2. 주요 구현 내용
- **DH Parameter & Forward Kinematics**:
  - 4개 링크에 대한 Denavit-Hartenberg 파라미터 정의
  - 각 링크 변환행렬(^1, T_1^2, T_2^3, T_3^4$) 및 엔드이펙터 최종 변환행렬(^4$) 심볼릭 유도
- **질량중심 속도 및 Jacobian**:
  - 각 링크의 질량중심 위치($) 및 자코비안 행렬을 통한 선속도($)와 각속도($\omega$) 연산
- **Lagrangian Dynamics & 토크 제어 방정식**:
  - 시스템 운동에너지($)와 위치에너지($)를 통한 라그랑지안( = K - P$) 계산
  - 관성 행렬 (	heta)$, 코리올리/원심력 행렬 (	heta, \dot{	heta})$, 중력 벡터 (	heta)$ 도출
  - 최종 관절 구동 토크 수식 $	au = M(	heta)\ddot{	heta} + C(	heta, \dot{	heta})\dot{	heta} + G(	heta)$ 유도

## 3. 기술 스택
- **Language**: MATLAB (Symbolic Math Toolbox)
- **Concepts**: Denavit-Hartenberg (DH) Parameter, Forward Kinematics, Jacobian, Lagrangian Dynamics, Computed Torque

## 4. 근거 파일
- `로봇제어/middle.m` (156줄의 완전한 MATLAB 심볼릭 연산 스크립트)
