#include<stdio.h>
#define is_overflow(a, b) \
   __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)
int main(){
    printf("%d",is_overflow (__INT_MAX__, 3) ? 0 : __INT_MAX__ + 3);
    return 0;
}