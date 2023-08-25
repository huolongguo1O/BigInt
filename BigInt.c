#include<stdio.h>
#include<stdlib.h>
#include <limits.h>
// check add overflow
int isAddOverflow(unsigned int x, unsigned int y)
{
    unsigned int sum = x + y;
    return sum < y||sum < x;
}

// define 
// use builtin first (gcc)
#define is_overflow(a, b) \
   __builtin_add_overflow_p (a, b, (__typeof__ ((a) + (b))) 0)

// notused
#define value(a) \
    i < a -> size ? a -> value : 0

// struct of big int    
typedef struct _BIG_INT{
    unsigned int value; //size if first
    struct _BIG_INT * next;
} BIG_INT;

// a+b
BIG_INT add(BIG_INT a,BIG_INT b){
    unsigned int sizea = (int)a.value < 0 ? -a.value : a.value ;
    unsigned int sizeb = (int)b.value < 0 ? -b.value : b.value ;
    unsigned int longer = sizea > sizeb ? sizea : sizeb ;
    BIG_INT ret ;
    ret.value = 0 ;
    BIG_INT * now =&ret;
    BIG_INT * _a = a.next ;
    BIG_INT * _b = b.next ;
    int over = 0 ;
    for(int i = 0; i < longer; i++){
        if(over){
            BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
            tmp -> value = (i < a.value ? _a -> value : 0) + (i < b.value ? _b -> value : 0) + 1;
            over = is_overflow((i < a.value ? _a -> value : 0) , (i < b.value ? _b -> value : 0)) ;
            if(!over)
                if(tmp -> value == 0)
                    over = 1 ;
            _a = i < a.value ? _a -> next : _a ;
            _b = i < b.value ? _b -> next : _b ;
            now -> next = tmp ;
            now = tmp ;
            ret.value++ ;
            continue;
        }
        BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
        tmp -> value = (i < a.value ? _a -> value : 0) + (i < b.value ? _b -> value : 0) ;
        over = is_overflow((i < a.value ? _a -> value : 0) , (i < b.value ? _b -> value : 0)) ;
        _a = i < a.value ? _a -> next : _a ;
        _b = i < b.value ? _b -> next : _b ;
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

// sub0(a) = -b
BIG_INT sub0(BIG_INT a){
    BIG_INT * ret = malloc(sizeof(BIG_INT));
    *ret = a;
    ret -> value = -a.value;
    BIG_INT * tmp = malloc(sizeof(BIG_INT));
    *tmp = *(ret->next) ;
    tmp -> value = (unsigned int)-(tmp -> value) ;
    for(int i = 0; i < abs(a.value)-1; i++){
        BIG_INT * org = tmp ;
        tmp = malloc(sizeof(BIG_INT));
        *tmp = *(org -> next) ;
        tmp -> value = (tmp -> value) ^ (unsigned int)-2;
    }
    return * ret;
}

// a-b
BIG_INT sub(BIG_INT a, BIG_INT b){
    return add(a, sub0(b));
}

BIG_INT from_hex(char * hex, int len){
	
}

int main(){
    BIG_INT a;
    BIG_INT b;
    BIG_INT a1;
    BIG_INT b1;
    a.next = &a1;
    b.next = &b1;
    a.value = 1;
    b.value = 1;
    a1.value = -1;
    b1.value = 114514;
    a1.next = (BIG_INT *)-1;
    b1.next = (BIG_INT *)-1;
    add(a,b);
    sub(a,b);
}
