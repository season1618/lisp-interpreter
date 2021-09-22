#include "bool.h"
#include "basic.h"
#include "print.h"

void print(long* p){
    if(atom(p) == t){
        printf("%d",*p);
    }else{
        if(atom(car(p)) == t){
            print(car(p));
        }else{
            printf("(");
            print(car(p));
            printf(")");
        }
        printf(" ");
        print(cdr(p));
    }
}