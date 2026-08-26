clc;
clear;

% DH 파라미터 정의
DH_params = [
    0,      -pi/2,   0,       0;       % 링크 1
    431.8e-3, 0,     -149.09e-3, 0;    % 링크 2
   -20.32e-3, pi/2,   0,       0;       % 링크 3
    0,      -pi/2,   433.07e-3, 0       % 링크 4
];

% 조인트 변수와 속도를 심볼릭 변수로 정의
syms theta1 theta2 theta3 theta4 real  % 각 조인트의 회전각
syms dtheta1 dtheta2 dtheta3 dtheta4 real % 각 조인트의 각속도
syms ddtheta1 ddtheta2 ddtheta3 ddtheta4 real % 각 조인트의 각가속도

% 중력 벡터 정의
g = [0; 0; -9.81]; % 중력 가속도 방향

% 조인트 변수 벡터 정의
theta = [theta1; theta2; theta3; theta4];
dtheta = [dtheta1; dtheta2; dtheta3; dtheta4];
ddtheta = [ddtheta1; ddtheta2; ddtheta3; ddtheta4];

% DH 변환 행렬을 계산하는 함수 정의
DH_transform = @(a, alpha, d, theta) [
    cos(theta), -sin(theta)*cos(alpha),  sin(theta)*sin(alpha), a*cos(theta);
    sin(theta),  cos(theta)*cos(alpha), -cos(theta)*sin(alpha), a*sin(theta);
    0,           sin(alpha),            cos(alpha),            d;
    0,           0,                     0,                     1
];

% 개별 변환 행렬 계산
T = cell(1, 4); % 각 링크의 변환 행렬을 저장할 셀 배열
for i = 1:4
    a = DH_params(i, 1);        
    alpha = DH_params(i, 2);    
    d = DH_params(i, 3);        
    theta_var = theta(i);       
    T{i} = DH_transform(a, alpha, d, theta_var); % 변환 행렬 계산
end

% 전체 변환 행렬 계산
T0_1 = T{1}; % 링크 1에 대한 변환 행렬
T1_2 = T{2}; % 링크 2에 대한 변환 행렬
T2_3 = T{3}; % 링크 3에 대한 변환 행렬
T3_4 = T{4}; % 링크 4에 대한 변환 행렬
T0_2 = simplify(T0_1 * T1_2); % 링크 2까지의 변환 행렬
T0_3 = simplify(T0_2 * T2_3); % 링크 3까지의 변환 행렬
T0_4 = simplify(T0_3 * T3_4); % 링크 4까지의 변환 행렬

% 질량 중심 위치 계산
p_c = cell(1, 4); % 각 링크의 질량 중심 위치를 저장할 셀 배열
for i = 1:4
    % 링크의 질량 중심이 해당 링크의 중심에 있다고 가정
    if i == 1
        T_c = T0_1; % 첫 번째 링크의 좌표
    elseif i == 2
        T_c = T0_2; % 두 번째 링크의 좌표
    elseif i == 3
        T_c = T0_3; % 세 번째 링크의 좌표
    else
        T_c = T0_4; % 네 번째 링크의 좌표
    end
    p_c{i} = T_c(1:3, 4); % 질량 중심 위치 벡터 추출
end

% 변환 행렬 출력
disp('Transformation Matrices:');
disp('T0_1 ='); disp(T0_1);
disp('T0_2 ='); disp(T0_2);
disp('T0_3 ='); disp(T0_3);
disp('T0_4 ='); disp(T0_4);

% 링크의 질량과 관성 텐서 정의 (예시값)
m = [5; 10; 7; 3]; % 각 링크의 질량 (단위: kg)
I = cell(1, 4); % 각 링크의 관성 텐서를 저장할 셀 배열
I{1} = diag([0.1, 0.1, 0.1]); % 링크 1의 inertia
I{2} = diag([0.2, 0.2, 0.2]); % 링크 2의 inertia
I{3} = diag([0.15, 0.15, 0.15]); % 링크 3의 inertia
I{4} = diag([0.05, 0.05, 0.05]); % 링크 4의 inertia

% 라그랑지 방법을 사용하여 동역학 계산
K = 0; % 운동 에너지 초기화
P = 0; % 위치 에너지 초기화
for i = 1:4
    % 질량 중심의 속도와 위치 계산
    R = T{i}(1:3, 1:3); % 현재 링크의 회전 행렬
    p = p_c{i};         % 질량 중심 위치
    v = simplify(jacobian(p, theta) * dtheta); % 질량 중심의 선속도 계산
    
    % inertia 변환
    I_rot = simplify(R * I{i} * R.');
    
    % 각속도 계산
    omega = simplify(jacobian(R(:), theta) * dtheta); % 각속도 계산
    omega = reshape(omega, 3, 3) * [0; 0; 1]; % 각속도 벡터화

    % 운동 에너지 계산
    K = K + 0.5 * m(i) * (v.' * v) + 0.5 * omega.' * I_rot * omega;
    
    % 위치 에너지 계산
    P = P + m(i) * g.' * p;
end

% 라그랑지안 계산
L = simplify(K - P); % 라그랑지안: 운동 에너지 - 위치 에너지

% 운동 방정식 (토크 계산)
tau = sym(zeros(4, 1)); % 토크 초기화
M = sym(zeros(4, 4)); % 관성 행렬 초기화
C = sym(zeros(4, 4)); % 코리올리 및 원심력 행렬 초기화
G = sym(zeros(4, 1)); % 중력 항 초기화

% 관성 행렬 M 계산
for i = 1:4
    for j = 1:4
        M(i, j) = simplify(diff(diff(L, dtheta(i)), dtheta(j))); % 라그랑지안으로부터 M 추출
    end
end

disp('Inertia Matrix (M):');
disp(M);

% 코리올리 및 원심력 행렬 C 계산
for i = 1:4
    for j = 1:4
        C(i, j) = 0; % 초기화
        for k = 1:4
            term1 = diff(M(i, j), theta(k));
            term2 = diff(M(i, k), theta(j));
            term3 = diff(M(j, k), theta(i));
            C(i, j) = C(i, j) + 0.5 * (term1 + term2 - term3) * dtheta(k);
        end
    end
end

C = simplify(C);
disp('Coriolis and Centrifugal Matrix (C):');
disp(C);

% 중력 항 G 계산
for i = 1:4
    G(i) = simplify(diff(P, theta(i))); % 위치 에너지로부터 G 추출
end

G = simplify(G);
disp('Gravity Vector (G):');
disp(G);

% 최종 토크 계산
tau = simplify(M * ddtheta + C * dtheta + G);

disp('Torques (tau):');
disp(tau);
