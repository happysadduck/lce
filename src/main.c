#include<stdio.h>
#include"core.h"

void test_case_1() {
    printf("Test case 1: zero acceleration, simple collision\n");
    Ship ship = {
        .p = {0, 0},
        .v = {1, 0},
        .a = {0, 0}
    };
    DamageSrc damage = {
        .o = {2, 0},
        .v = {0, 0},
        .r = 1,
        .ev = 0
    };
    double out[4];
    int n = get_collision_tick(&ship, &damage, out);
    printf("Found %d solutions: ", n);
    for (int i = 0; i < n; ++i) printf("%.12f ", out[i]);
    printf("\n");
    // 预期：t=1 时相遇（飞船从0到1，圆在2，半径1，正好相切）
    if (n == 1 && fabs(out[0] - 1.0) < 1e-10) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void test_case_2() {
    printf("Test case 2: zero acceleration, expanding circle\n");
    Ship ship = {
        .p = {0, 0},
        .v = {1, 0},
        .a = {0, 0}
    };
    DamageSrc damage = {
        .o = {2, 0},
        .v = {0, 0},
        .r = 0.5,
        .ev = 0.5
    };
    double out[4];
    int n = get_collision_tick(&ship, &damage, out);
    printf("Found %d solutions: ", n);
    for (int i = 0; i < n; ++i) printf("%.12f ", out[i]);
    printf("\n");
    // 解方程：|(t-2)| = 0.5+0.5t => t-2 = ±(0.5+0.5t)
    // 正号：t-2 = 0.5+0.5t => 0.5t = 2.5 => t=5（超出TICK_STEP=1，忽略）
    // 负号：t-2 = -0.5-0.5t => 1.5t = 1.5 => t=1
    if (n == 1 && fabs(out[0] - 1.0) < 1e-10) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void test_case_3() {
    printf("Test case 3: non-zero acceleration\n");
    Ship ship = {
        .p = {0, 0},
        .v = {0, 0},
        .a = {1, 0}  // 加速度为1
    };
    DamageSrc damage = {
        .o = {1, 0},
        .v = {0, 0},
        .r = 0,
        .ev = 0
    };
    double out[4];
    int n = get_collision_tick(&ship, &damage, out);
    printf("Found %d solutions: ", n);
    for (int i = 0; i < n; ++i) printf("%.12f ", out[i]);
    printf("\n");
    // 飞船位置：0.5*t^2，圆位置固定1，半径0，相遇条件：0.5*t^2 = 1 => t = sqrt(2) ≈ 1.414，大于TICK_STEP=1，应无解
    if (n == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

void test_case_4() {
    printf("Test case 4: contact at t=0\n");
    Ship ship = {
        .p = {1, 0},
        .v = {0, 0},
        .a = {0, 0}
    };
    DamageSrc damage = {
        .o = {0, 0},
        .v = {0, 0},
        .r = 1,
        .ev = 0
    };
    double out[4];
    int n = get_collision_tick(&ship, &damage, out);
    printf("Found %d solutions: ", n);
    for (int i = 0; i < n; ++i) printf("%.12f ", out[i]);
    printf("\n");
    // 应包含 t=0
    if (n == 1 && fabs(out[0]) < 1e-10) {
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }
}

int main() {
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    return 0;
}