# 4자유도 매니퓰레이터 기구학 및 동역학 토크 해석

## 1. 프로젝트 요약
4축 다자유도 로봇 매니퓰레이터의 기하학적 구조를 수학적으로 정의하고, 자코비안 및 라그랑지안 역학을 기반으로 관성, 코리올리, 중력 보상 및 관절 구동 토크 방정식을 유도한 시뮬레이션 프로젝트입니다.

## 2. 주요 구현 내용
- **DH Parameter & Forward Kinematics**:
  - 4개 링크에 대한 Denavit-Hartenberg 파라미터를 정의했습니다.
  - 각 링크 변환행렬($T_0^1$, $T_1^2$, $T_2^3$, $T_3^4$)과 엔드이펙터 최종 변환행렬($T_0^4$)을 심볼릭으로 유도했습니다.
- **질량중심 속도 및 Jacobian**:
  - 각 링크의 질량중심 위치($p_c$)를 정의하고, 자코비안 행렬을 통해 선속도($v = J\dot{q}$)와 각속도($\omega$)를 계산했습니다.
- **Lagrangian Dynamics & 토크 제어 방정식**:
  - 시스템 운동에너지($K$)와 위치에너지($P$)를 통해 라그랑지안($L = K - P$)을 구성했습니다.
  - 관성 행렬 $M(\theta)$, 코리올리/원심력 행렬 $C(\theta, \dot{\theta})$, 중력 벡터 $G(\theta)$를 분리했습니다.
  - 최종 관절 구동 토크 수식 $\tau = M(\theta)\ddot{\theta} + C(\theta, \dot{\theta})\dot{\theta} + G(\theta)$를 유도했습니다.

## 3. 기술 스택
- **Language**: MATLAB (Symbolic Math Toolbox)
- **Concepts**: Denavit-Hartenberg (DH) Parameter, Forward Kinematics, Jacobian, Lagrangian Dynamics, Computed Torque

## 4. 근거 파일
- `로봇제어/middle.m` (156줄의 완전한 MATLAB 심볼릭 연산 스크립트)
