#include<stdio.h>
#include<stdlib.h>
#define is_overflow(a, b) \
   __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)

typedef struct _BIG_INT{
    unsigned int value; //size if first
    struct _BIG_INT * next;
} BIG_INT;
BIG_INT add(BIG_INT a,BIG_INT b){
    unsigned int sizea = (int)a.value < 0 ? -a.value : a.value ;
    unsigned int sizeb = (int)b.value < 0 ? -b.value : b.value ;
    unsigned int longer = sizea > sizeb ? sizea : sizeb ;
    BIG_INT ret ;
    ret.value = 0 ;
    BIG_INT * now =&ret;
    BIG_INT * _a = &a ;
    BIG_INT * _b = &b ;
    int over;
    for(int i = 0; i < longer; i++){
        if(over){
            BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
            tmp -> value = _a -> value + _b -> value + 1;
            over = is_overflow(_a -> value , _b -> value) ;
            if(!over)
                if(tmp -> value == 0)
                    over = 1 ;
            _a = _a -> next ;
            _b = _b -> next ;
            now -> next = tmp ;
            now = tmp ;
            ret.value++ ;
            continue;
        }
        BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
        tmp -> value = _a -> value + _b -> value ;
        over = is_overflow(_a -> value , _b -> value) ;
        _a = _a -> next ;
        _b = _b -> next ;
        now -> next = tmp ;
        now = tmp ;
        ret.value++ ;
    }
    if(over){
        BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
        tmp -> value = 1 ;
        now -> next = tmp ;
        now = tmp ;
        ret.value++ ;
    }
    return ret;
}

void sub0(BIG_INT a){
    BIG_INT * now = &a ;
    BIG_INT ret = a;
    BIG_INT * tmp = malloc(sizeof(BIG_INT));
    *tmp = *ret.next ;
    tmp -> value = (unsigned int)-(tmp -> value);
    for(int i = 0; i < abs(a.value)-1; i++){
        now = now -> next;
        now -> value = (now -> value) ^ 0xFFFFFFFE;
    }
}

BIG_INT sub(BIG_INT a, BIG_INT b){

}
