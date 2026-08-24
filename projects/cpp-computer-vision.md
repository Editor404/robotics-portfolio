# C++ 컴퓨터비전 영상처리 핵심 알고리즘 구현

## 1. 프로젝트 개요
- **수행 교과**: 컴퓨터비전 (2023년 1학기)
- **개발 환경**: C++ (Modern C++), Qt Creator, MinGW / GCC
- **근거 파일**: `컴퓨터비전/2020741023_이윤범_7주차/*.cpp`, `컴퓨터비전/2020741023_이윤범.pdf`

---

## 2. 주요 구현 알고리즘 및 수학적 원리

### 1) Canny Edge Detection (5단계 파이프라인 직접 구현)
1. **Gaussian Smoothing**: $5	imes 5$ 가우시안 필터 ($\sigma=1.4$)로 고주파 노이즈 제거
2. **Gradient Calculation**: $x, y$ 방향 소벨 마스크 합성으로 기울기 크기 $M(x,y) = \sqrt{G_x^2 + G_y^2}$ 및 방향 $	heta = rctan(G_y / G_x)$ 연산
3. **Non-Maximum Suppression (NMS)**: 8방향(0°, 45°, 90°, 135°)으로 양자화하여 그래디언트 방향에서 극대값이 아닌 픽셀 억제
4. **Double Thresholding**: $T_{high}$ (강한 엣지)와 $T_{low}$ (약한 엣지)로 분할
5. **Hysteresis Edge Tracking**: 강한 엣지와 8-이웃으로 연결된 약한 엣지만 유효 엣지로 보존

### 2) Harris Corner Detection
- 국소 영역 구조 텐서(Structure Tensor) 행렬 $M$ 구성:
  $$M = \sum_{(x,y)} w(x,y) egin{bmatrix} I_x^2 & I_x I_y \ I_x I_y & I_y^2 \end{bmatrix}$$
- 코너 응답 점수: $R = \det(M) - k \cdot (	ext{trace}(M))^2$ ($k=0.04$)
- 국소 최대점(Local Maxima) 탐색을 통해 코너 좌표 추출

### 3) Circle Hough Transform & Otsu 이진화
- $(x-a)^2 + (y-b)^2 = r^2$ 파라미터 공간의 3차원 누적 배열(Accumulator Array) 투표로 원 검출
- 클래스 간 분산(Between-class variance) $\sigma_B^2(t) = \omega_0(t)\omega_1(t)(\mu_0(t) - \mu_1(t))^2$을 최대화하는 Otsu 최적 임계치 자동 계산

---

## 3. 핵심 C++ 소스코드 스니펫

```cpp
// Non-Maximum Suppression (NMS) C++ 구현 발췌
void NonMaxSuppression(const cv::Mat& magnitude, const cv::Mat& direction, cv::Mat& nms) {
    int rows = magnitude.rows;
    int cols = magnitude.cols;
    nms = cv::Mat::zeros(rows, cols, CV_32F);

    for (int y = 1; y < rows - 1; y++) {
        for (int x = 1; x < cols - 1; x++) {
            float angle = direction.at<float>(y, x);
            float mag = magnitude.at<float>(y, x);
            float q = 0.0f, r = 0.0f;

            // 0도 (수평)
            if ((angle >= -22.5 && angle < 22.5) || (angle >= 157.5 || angle <= -157.5)) {
                q = magnitude.at<float>(y, x + 1);
                r = magnitude.at<float>(y, x - 1);
            }
            // 45도 (대각)
            else if ((angle >= 22.5 && angle < 67.5) || (angle >= -157.5 && angle < -112.5)) {
                q = magnitude.at<float>(y + 1, x - 1);
                r = magnitude.at<float>(y - 1, x + 1);
            }
            // 90도 (수직)
            else if ((angle >= 67.5 && angle < 112.5) || (angle >= -112.5 && angle < -67.5)) {
                q = magnitude.at<float>(y + 1, x);
                r = magnitude.at<float>(y - 1, x);
            }
            // 135도 (대각)
            else {
                q = magnitude.at<float>(y - 1, x - 1);
                r = magnitude.at<float>(y + 1, x + 1);
            }

            if (mag >= q && mag >= r) nms.at<float>(y, x) = mag;
            else nms.at<float>(y, x) = 0.0f;
        }
    }
}
```
