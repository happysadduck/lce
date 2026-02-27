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