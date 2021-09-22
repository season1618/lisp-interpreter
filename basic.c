#include "bool.h"
#include "basic.h"

long* atom(long* x){
    if(*(x + 1) == *nil) return t;
    else return nil;
}

long* eq(long* x, long* y){
    if(atom(x) == t){
        if(atom(y) == t){
            if(*x == *y) return t;
            else return nil;
        }else return nil;
    }else{
        if(atom(y) == t) return nil;
        else{
            if(eq((long*)*x, (long*)*y) == t && eq((long*)*(x + 1), (long*)*(y + 1)) == t) return t;
            else return nil;
        }
    }
}

long* car(long* x){
    if(atom(x) == t){
        //printf("error : argument of car is pair.\n");
        return nil;
    }else return (long*)*x;
}

long* cdr(long* x){
    if(atom(x) == t){
        //printf("error : argument of cdr is pair.\n");
        return nil;
    }else return (long*)*(x + 1);
}

long* cons(long* x, long* y){
    long* p = calloc(2, sizeof(long));
    *p = (long)x;
    *(p + 1) = (long)y;
    return p;
}