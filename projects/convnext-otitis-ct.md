# ConvNeXt 기반 측두골 CT 중이염 다중 레이블 분류

## 요약
측두골 CT PNG 슬라이스에서 우측/좌측 측두골 영역과 우측/좌측 중이염 여부를 동시에 예측하는 4출력 다중 레이블 분류 모델입니다.

## 구현 내용
- Backbone: ImageNet-1K 사전학습 ConvNeXt-Tiny
- Head: Dropout + 4차원 Linear classifier
- Loss: 클래스 불균형 보정을 위한 `pos_weight` 적용 BCEWithLogitsLoss
- Optimizer: AdamW
- Data split: 환자 단위 train/validation 분리로 데이터 누수 완화
- Augmentation: 약한 affine augmentation, 좌우 라벨 의미 보존을 위해 horizontal flip 제외
- Inference: 클래스별 threshold 적용, 슬라이스 연속성 기반 후처리

## 성과
- 평가 슬라이스 수: 3,320
- Accuracy: 88.86%
- Macro F1-score: 0.8848
- 환산 점수: 88.9 / 100.0

## 한계 분석
우측 중이염 클래스 recall이 낮아 불균형 데이터에서 클래스별 평가와 threshold/후처리 개선이 필요함을 확인했습니다.

## 기술 키워드
PyTorch, ConvNeXt-Tiny, Medical Image Classification, Multi-label Classification, BCEWithLogitsLoss, AdamW, Post-processing

## 근거 파일
- [`evidence/convnext-otitis-ct/model.py`](../evidence/convnext-otitis-ct/model.py)
- [`evidence/convnext-otitis-ct/inference.py`](../evidence/convnext-otitis-ct/inference.py)
- [`evidence/convnext-otitis-ct/submission_validation.csv`](../evidence/convnext-otitis-ct/submission_validation.csv)
- [`evidence/convnext-otitis-ct/otitis-ct-report-redacted.docx`](../evidence/convnext-otitis-ct/otitis-ct-report-redacted.docx)
