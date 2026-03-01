```c
/*视野与隐蔽的测试代码, 适用于0.1pre1*/
#include<stdio.h>
#include"core.h"

void print_discovery(
    int found,
    Discovery*d
){
    if(found)
        printf(
            "Found at tick %ld: "
            "(%.2f, %.2f)\n",
            d->tick,
            d->p.x,
            d->p.y
        );
    else
        printf("Not found\n");
}

int main(){
    Ship observer={0};
    Ship observed={
        {1,0}, {0, 0}, {-0.2,0}
    };
    Plan p5={0, 0, 5, {{0.2, 0}, 0}};
    Plan p10={0, &p5, 10, {{0, 0}, 0}};
    p5.next=&p10;
    Discovery out;
    int found=get_discovery_general(
        &p10, &observer, &observed,
        12, 0, &out
    );
    print_discovery(found, &out);
    return 0;
}
```
```c
/*四次方程的测试代码, 适用于0.1pre2*/
#include<stdio.h>
#include"core.h"
#include"poly.h"

static double quartic_val(
    double a4, double a3,
    double a2, double a1,
    double a0, double x)
{
    double result = a4;
    result = result * x + a3;
    result = result * x + a2;
    result = result * x + a1;
    return result * x + a0;
}

int main() {
    double roots[4];
    int n;
    
    // 测试1: (x-1)(x-2)(x-3)(x-4) = x^4 - 10x^3 + 35x^2 - 50x + 24
    printf("Test 1: (x-1)(x-2)(x-3)(x-4) in [0, 5]\n");
    n = solve_quartic_roots(1, -10, 35, -50, 24, roots, 5.0);
    printf("Found %d roots:\n", n);
    for (int i = 0; i < n; i++) {
        double val = quartic_val(1, -10, 35, -50, 24, roots[i]);
        printf("  x = %.15f, f(x) = %.6e\n", roots[i], val);
    }
    printf("\n");
    
    // 测试2: x^4 - 5x^2 + 4 = (x^2-1)(x^2-4) = (x-1)(x+1)(x-2)(x+2), 在[0,3]内应有1,2
    printf("Test 2: x^4 - 5x^2 + 4 in [0, 3]\n");
    n = solve_quartic_roots(1, 0, -5, 0, 4, roots, 3.0);
    printf("Found %d roots:\n", n);
    for (int i = 0; i < n; i++) {
        double val = quartic_val(1, 0, -5, 0, 4, roots[i]);
        printf("  x = %.15f, f(x) = %.6e\n", roots[i], val);
    }
    printf("\n");
    
    // 测试3: 重根 (x-2)^2(x-3)^2 = x^4 - 10x^3 + 37x^2 - 60x + 36
    printf("Test 3: (x-2)^2(x-3)^2 in [0, 5] (double roots at 2,3)\n");
    n = solve_quartic_roots(1, -10, 37, -60, 36, roots, 5.0);
    printf("Found %d roots:\n", n);
    for (int i = 0; i < n; i++) {
        double val = quartic_val(1, -10, 37, -60, 36, roots[i]);
        printf("  x = %.15f, f(x) = %.6e\n", roots[i], val);
    }
    printf("\n");
    
    // 测试4: 无根 x^4 + 1 = 0 在 [0, 2]
    printf("Test 4: x^4 + 1 in [0, 2] (no real roots)\n");
    n = solve_quartic_roots(1, 0, 0, 0, 1, roots, 2.0);
    printf("Found %d roots\n", n);
    
    return 0;
}
```
```c
/*碰撞检测的基础测试, 可能适用于0.1pre3*/
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
```
