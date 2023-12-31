#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
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
	struct _BIG_INT * pre;
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
            tmp -> next = (BIG_INT *)-1;
            tmp -> value = (i < a.value ? _a -> value : 0) + (i < b.value ? _b -> value : 0) + 1;
            over = is_overflow((i < a.value ? _a -> value : 0) , (i < b.value ? _b -> value : 0)) ;
            if(!over)
                if(tmp -> value == 0)
                    over = 1 ;
            _a = i < a.value ? _a -> next : _a ;
            _b = i < b.value ? _b -> next : _b ;
            now -> next = tmp ;
            tmp -> pre = now ;
            now = tmp ;
            ret.value++ ;
            continue;
        }
        BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
        tmp -> next = (BIG_INT *)-1;
        tmp -> value = (i < a.value ? _a -> value : 0) + (i < b.value ? _b -> value : 0) ;
        over = is_overflow((i < a.value ? _a -> value : 0) , (i < b.value ? _b -> value : 0)) ;
        _a = i < a.value ? _a -> next : _a ;
        _b = i < b.value ? _b -> next : _b ;
        now -> next = tmp ;
        tmp -> pre = now ;
        now = tmp ;
        ret.value++ ;
    }
    if(over){
        BIG_INT * tmp = malloc(sizeof(BIG_INT)) ;
        tmp -> next = (BIG_INT *)-1;
        tmp -> value = 1 ;
        now -> next = tmp ;
        tmp -> pre = now ;
        now = tmp ;
        ret.value++ ;
    }
    return ret;
}

// sub0(a) = -b
BIG_INT sub0(BIG_INT a){
    BIG_INT * ret = malloc(sizeof(BIG_INT));
    ret-> next = (BIG_INT *)-1;
    *ret = a;
    ret -> value = -a.value;
    BIG_INT * tmp = malloc(sizeof(BIG_INT));
    tmp-> next = (BIG_INT *)-1;
    *tmp = *(ret->next) ;
    tmp -> value = (unsigned int)-(tmp -> value) ;
    for(int i = 0; i < abs(a.value)-1; i++){
        BIG_INT * org = tmp ;
        tmp = malloc(sizeof(BIG_INT));
        tmp -> next = (BIG_INT *)-1;
        *tmp = *(org -> next) ;
        
        tmp -> value = (tmp -> value) ^ (unsigned int)-1;
    }
    return * ret;
}

// a-b
BIG_INT sub(BIG_INT a, BIG_INT b){
    return add(a, sub0(b));
}

// convert from hex, len exclude \0
BIG_INT from_hex(char * hex/*指针-指向传入变量地址*/, int len){
	BIG_INT ret ;//返回值
	ret.value = 0;
	ret.next=(BIG_INT *)-1;
    char tmp[9] = {0} ;//读取变量
    BIG_INT *now = &ret ;//指针-指向返回值
    int i = 0;
    for(i = len - 8; i > 0; i-=8){
        for(int j=0;j<8;j++){
            tmp[j]=hex[i+j];// 你这样还不如memcpy（？）6
        }//从字符串内读取8位
        //long long mid = hex[i];
        //*tmp = mid;
        BIG_INT * btmp = malloc(sizeof(BIG_INT));
        btmp-> next = (BIG_INT *)-1;
        char * rubbish;//检测是否合法的变量（？）
        btmp -> value = strtol(tmp, &rubbish, 16);
        //btmp指针指向的value子变量赋值为16进制转化后的数值
        if(*rubbish != '\0')//包含其他字符
            continue;//停止执行下面的代码，继续遍历
        now -> next = btmp ;//ret.next指向btmp
        btmp -> pre = now ;
        now = btmp ;//更新当前读取节点为&btmp
        ret.value++;
    }
    memset(tmp, 0, 9);
    i+=7;
    int j = 7;
    for(; i >= 0; i--,j--){
        
        tmp[j] = hex[i];
    }
    BIG_INT * btmp = malloc(sizeof(BIG_INT));
    btmp-> next = (BIG_INT *)-1;
    char * rubbish;//检测是否合法的变量（？）
    btmp -> value = strtol(tmp+j+1, &rubbish, 16);
    //btmp指针指向的value子变量赋值为16进制转化后的数值
    now -> next = btmp ;//ret.next指向btmp
    btmp -> pre = now ;
    now = btmp ;
    ret.value++;
    return ret;
    
}
char* to_hex(BIG_INT bi){
	char * buf = malloc(sizeof(char)*2*bi.value*sizeof(int)+1);
	memset(buf, 0, sizeof(char)*bi.value*sizeof(int)+1);
    char tmp[9];
    BIG_INT* now = bi.next;
    int cnt = 0;
    int ptr = sizeof(char)*2*bi.value*sizeof(int);
    int len;
    while(now!=(BIG_INT *)-1){// 暂时头结点的value即长度 啊？
		ptr -= 8 ;
        sprintf(tmp, "%8X", now->value);//转化为字符串
        len=strlen(tmp);
        for(int i = len-1;i >= 0; i--){
            buf[i+(ptr)]=tmp[i];
        }
        now=now->next;//应该是这样
        //cnt++;
    }
    return buf;
}
int main(){
    char hex1[160],hex2[160],out[200];
    scanf("%s",hex1);
    scanf("%s",hex2);
    BIG_INT a ;
    BIG_INT b;
    a = from_hex(hex1,strlen(hex1));
    b = from_hex(hex2,strlen(hex2));
    
    printf("%s\n",to_hex(sub0(a,b)));
    //sub(a,b);
}
