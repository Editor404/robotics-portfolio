import os
import pandas as pd
import torch
from torchvision import transforms
from PIL import Image
from tqdm import tqdm

# model.py에서 본인이 설계한 모델을 가져옵니다.
from model import MyModel

# ==========================================================
# [추론 설정]
# 학습 시 설정한 IMAGE_SIZE 및 전처리와 반드시 일치해야 합니다.
# ==========================================================
MODEL_PATH = '/home/keivn/DL_work/submissions/best_model.pth'
IMAGE_SIZE = 300
THRESHOLDS = [0.20, 0.24, 0.95, 0.06]
SMOOTH_MAX_GAP = 1
SMOOTH_MIN_POS_RUN = 8
RT_OTITIS_MIN_POS_RUN = 5

# 표준 전처리 과정 (학습 시와 동일하게 유지)
INFERENCE_TRANSFORMS = transforms.Compose([
    transforms.Resize((IMAGE_SIZE, IMAGE_SIZE)),
    transforms.ToTensor(),
    transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
])


def fill_short_gaps(values, max_gap):
    #1 사이에 낀 짧은 0 구간을 병변 연속 구간으로 채우기
    if max_gap <= 0:
        return values[:]

    smoothed = values[:]
    n = len(smoothed)
    i = 0
    while i < n:
        if smoothed[i] != 0:
            i += 1
            continue

        j = i
        while j < n and smoothed[j] == 0:
            j += 1

        gap_len = j - i
        has_left_positive = i > 0 and smoothed[i - 1] == 1
        has_right_positive = j < n and smoothed[j] == 1
        if has_left_positive and has_right_positive and gap_len <= max_gap:
            for k in range(i, j):
                smoothed[k] = 1

        i = j

    return smoothed


def remove_short_positive_runs(values, min_run):
    #너무 짧게 튀는 1 구간을 false positive로 보고 제거
    if min_run <= 1:
        return values[:]

    smoothed = values[:]
    n = len(smoothed)
    i = 0
    while i < n:
        if smoothed[i] != 1:
            i += 1
            continue

        j = i
        while j < n and smoothed[j] == 1:
            j += 1

        if j - i < min_run:
            for k in range(i, j):
                smoothed[k] = 0

        i = j

    return smoothed


def apply_slice_smoothing(df, slice_cols):
    #CT 병변은 인접 slice에서 연속되는 경향이 있으므로 행별 132-slice prediction sequence에 short-run 제거 후 gap-fill
    for idx, row in df.iterrows():
        valid_cols = []
        values = []

        for col in slice_cols:
            if pd.notna(row[col]):
                valid_cols.append(col)
                values.append(int(float(row[col])))

        if not values:
            continue

        min_run = SMOOTH_MIN_POS_RUN
        if row['R/L'] == 'Rt' and row['Image number'] == 'otitis media':
            min_run = RT_OTITIS_MIN_POS_RUN

        values = remove_short_positive_runs(values, min_run)
        values = fill_short_gaps(values, SMOOTH_MAX_GAP)

        for col, value in zip(valid_cols, values):
            df.at[idx, col] = int(value)

    return df


def run_inference():
    # 경로 설정 (현재 폴더 또는 상위 폴더의 data 탐색)
    DATA_ROOT = "./data"
    if not os.path.exists(DATA_ROOT):
        DATA_ROOT = "/home/keivn/DL_work/data"
        
    TEMPLATE_PATH = "submission_template.csv"
    if not os.path.exists(TEMPLATE_PATH):
        TEMPLATE_PATH = "/home/keivn/DL_work/submissions/submission_template.csv"
        
    OUTPUT_PATH = "/home/keivn/DL_work/submissions/submission_validation.csv"
    
    if not os.path.exists(MODEL_PATH):
        print(f"Error: 모델 가중치 파일({MODEL_PATH})을 찾을 수 없습니다. 먼저 학습을 완료하십시오.")
        return

    if not os.path.exists(TEMPLATE_PATH):
        print(f"Error: 템플릿 파일({TEMPLATE_PATH})이 없습니다. 배포된 파일을 확인하십시오.")
        return

    if torch.cuda.is_available():
        device = torch.device("cuda")
        torch.backends.cudnn.benchmark = True
        print(f"사용 디바이스: cuda - {torch.cuda.get_device_name(0)}")
        print(f"CUDA version: {torch.version.cuda}")
    else:
        device = torch.device("cpu")
        print("사용 디바이스: cpu (CUDA를 사용할 수 없습니다)")

    # 모델 초기화 및 가중치 로드
    model = MyModel().to(device)
    model.load_state_dict(torch.load(MODEL_PATH, map_location=device, weights_only=True))
    model.eval()

    # 템플릿 로드
    df = pd.read_csv(TEMPLATE_PATH)
    slice_cols = [str(i) for i in range(1, 133)]
    
    print("검증 세트 추론을 시작합니다...")

    prob_cache = {}
    thresholds = torch.tensor(THRESHOLDS, device=device)

    for idx, row in tqdm(df.iterrows(), total=len(df)):
        p_id = str(int(row['No']))
        p_dir = os.path.join(DATA_ROOT, p_id)
        if not os.path.exists(p_dir):
            p_dir = os.path.join(DATA_ROOT, "val", p_id)
            
        img_dir = os.path.join(p_dir, "PNG")
        if not os.path.exists(img_dir): continue

        for col in slice_cols:
            img_name = f"{int(col):04d}.png"
            img_path = os.path.join(img_dir, img_name)
            
            if os.path.exists(img_path):
                try:
                    cache_key = (p_id, col)
                    if cache_key not in prob_cache:
                        # 이미지 로드 및 전처리
                        image = Image.open(img_path).convert('RGB')
                        input_tensor = INFERENCE_TRANSFORMS(image).unsqueeze(0).to(device)

                        # 추론
                        with torch.no_grad():
                            output = model(input_tensor)

                        prob_cache[cache_key] = torch.sigmoid(output)[0]

                    probs = prob_cache[cache_key]
                    preds = (probs > thresholds).int()

                    rl = row['R/L']
                    img_num = row['Image number']

                    if img_num == 'temporal area':
                        pred_val = preds[0] if rl == 'Rt' else preds[1]
                    elif img_num == 'otitis media':
                        pred_val = preds[2] if rl == 'Rt' else preds[3]
                    # 결과 기입
                    df.at[idx, col] = int(pred_val)
                except Exception as e:
                    print(f"Error (Patient {p_id}, Slice {col}): {e}")
                    continue
                    
    df = apply_slice_smoothing(df, slice_cols)

    # 최종 결과 저장
    df.to_csv(OUTPUT_PATH, index=False)
    print(f"\n추론 완료: '{OUTPUT_PATH}' 파일이 생성되었습니다.")

if __name__ == "__main__":
    run_inference()
