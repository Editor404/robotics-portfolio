# Physical AI / OpenVLA 기반 RaccoonBot 그리퍼 파이프라인 확장

> **RaccoonBot MuJoCo 그리퍼 환경에서 grasp/push/pick-and-place 데이터를 생성하고, RLDS/TFDS 변환 및 OpenVLA LoRA 학습·추론까지 연결한 Physical AI 프로젝트**

- **개발 기간**: 2026.06 (피지컬 AI)
- **GitHub 근거**: [Editor404/openVLA-with-Raccoonbot](https://github.com/Editor404/openVLA-with-Raccoonbot) (`피지컬ai/github.txt`)
- **주요 역할**: RaccoonBot 그리퍼 MuJoCo 환경 확장, grasp 데이터 생성 안정화, 멀티태스크 데이터셋 구성, OpenVLA용 RLDS/TFDS 변환, action label/그리퍼 convention 정합성 개선, 테스트 및 로그 기반 검증
- **기술 스택**: `Python`, `MuJoCo`, `OpenVLA`, `RLDS/TFDS`, `LoRA Fine-tuning`, `Robot Grasping`, `IK`, `Dataset Pipeline`, `Simulation QA`

---

## 1. 문제 정의

기존 RaccoonBot 파이프라인은 제한된 물체/명령 조합의 grasp 중심 데이터에 가까워, 실제 Physical AI 과제에서 요구되는 다양한 작업 지시와 물체 조건을 충분히 반영하기 어려웠습니다. 또한 OpenVLA 학습용 action label과 시뮬레이터 수집기의 gripper 표현이 어긋나면 학습 데이터는 만들어져도 실제 추론/제어 단계에서 안전하지 않은 명령이 발생할 수 있습니다.

본 프로젝트의 목표는 **그리퍼가 물체에 접근하고 잡고 옮기는 demonstration을 물리적으로 타당하게 수집**하고, 이를 **OpenVLA가 학습 가능한 task-balanced 데이터셋**으로 변환하는 것이었습니다.

---

## 2. 시스템 파이프라인

```text
MuJoCo RaccoonBot + Gripper
    │
    ├─ grasp / push / pick-and-place demonstration 수집
    │   ├─ 색상: red, blue, green, yellow
    │   ├─ 형상: cylinder, cube, sphere
    │   └─ 자연어 명령 템플릿 다양화
    │
    ├─ 안전/품질 필터링
    │   ├─ open-gripper approach 후 grasp-height에서 close
    │   ├─ MuJoCo contact 기반 성공 판정
    │   ├─ IK precheck 및 workspace clipping
    │   └─ 비정상 end-effector delta transition 저장 방지
    │
    ├─ RLDS intermediate 변환
    ├─ TFDS build 및 task-balanced mixture 등록
    └─ OpenVLA LoRA 학습·추론 서버/클라이언트 검증
```

---

## 3. 핵심 구현

### 3.1 그리퍼 demonstration 물리 타당성 개선

- 물체 접근 중에는 gripper를 연 상태로 유지하고, grasp-height 조건에 도달한 뒤에만 닫도록 수정했습니다.
- 인위적으로 물체를 그리퍼에 붙이는 방식을 기본 비활성화하고, MuJoCo contact/physics 기반으로 성공 여부를 판단했습니다.
- 실패한 배치나 비정상 transition은 전체 수집을 중단하지 않고 해당 시도만 재시도하도록 구성했습니다.

### 3.2 멀티태스크 데이터셋 확장

- 기존 grasp 중심 데이터에 `push`, `pick_and_place` 작업을 추가했습니다.
- 4개 색상 × 3개 형상 × 3개 작업 조합을 고려해 수집하고, 작업별 자연어 instruction template을 추가했습니다.
- 작업·색상·형상 조합별 균형 샘플링과 중단 후 재개 가능한 episode ID 복구 로직을 구현했습니다.

### 3.3 OpenVLA action label 정합성 수정

- idle frame 제거 후 여러 원본 프레임 이동량이 하나의 action으로 합쳐지는 문제를 수정했습니다.
- 각 observation의 action을 **바로 다음 원본 프레임까지의 이동량**으로 기록하도록 변환기를 개선했습니다.
- 수집기 gripper label `0=open, 1=close`를 OpenVLA convention `1=open, 0=close`로 변환했습니다.

---

## 4. 정량 결과 및 검증

| 항목 | 결과 |
| :--- | :--- |
| `raccoon_grasp` | train 1,080 / validation 120 episodes |
| `raccoon_push` | train 146 / validation 16 episodes |
| `raccoon_pick_and_place` | train 131 / validation 15 episodes |
| action safety | 축별 최대 이동량 약 4.9 mm 이하 |
| OpenVLA 학습 | `openvla/openvla-7b` LoRA rank 32 학습 로그 및 checkpoint 저장 확인 |

검증 근거는 공개 저장소의 `README.md`, `docs/short_report.md`, `docs/assignment_worklog.md`, `results/logs/`, `Mujoco/test_*.py`에 정리되어 있습니다.

---

## 5. 본인 기여 요약

1. RaccoonBot MuJoCo 그리퍼 환경에서 물체·색상·작업 다양성을 확장했습니다.
2. grasp/push/pick-and-place demonstration을 OpenVLA 학습용 데이터로 변환하는 파이프라인을 정리했습니다.
3. 그리퍼 open/close convention과 action delta label을 안전 제어 관점에서 정합화했습니다.
4. IK precheck, workspace clipping, transition safety filter, 재개 가능한 balanced collection으로 데이터 수집 안정성을 높였습니다.
5. 테스트, 로그, episode visualization을 함께 남겨 재현 가능한 제출/포트폴리오 근거로 만들었습니다.

---

## 6. 대표 근거 파일

- [`README.md`](https://github.com/Editor404/openVLA-with-Raccoonbot/blob/main/README.md): 전체 파이프라인 및 실행 방법
- [`Mujoco/raccoon_env.py`](https://github.com/Editor404/openVLA-with-Raccoonbot/blob/main/Mujoco/raccoon_env.py): RaccoonBot 시뮬레이션 환경 및 4D/7D action 실행
- [`Mujoco/raccoon_grasp_multicolor_scene_dataset.py`](https://github.com/Editor404/openVLA-with-Raccoonbot/blob/main/Mujoco/raccoon_grasp_multicolor_scene_dataset.py): grasp 데이터 생성
- [`Mujoco/raccoon_multitask_colored_objects_dataset.py`](https://github.com/Editor404/openVLA-with-Raccoonbot/blob/main/Mujoco/raccoon_multitask_colored_objects_dataset.py): push/pick-and-place 멀티태스크 데이터 생성
- [`Mujoco/rlds_dataset_builder/build_task_balanced_tfds.py`](https://github.com/Editor404/openVLA-with-Raccoonbot/blob/main/Mujoco/rlds_dataset_builder/build_task_balanced_tfds.py): task-balanced TFDS 빌드
- [`results/`](https://github.com/Editor404/openVLA-with-Raccoonbot/tree/main/results): 로그 및 episode visualization
