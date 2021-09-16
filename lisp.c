#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

FILE* fp;

long *t, *nil;

long* atom(long* x);
long* eq(long* x, long* y);
long* car(long* x);
long* cdr(long* y);
long* cons(long* x, long* y);

bool equal(char* a, char* b, int n);
long* number(long i);
char* function();
long* parentheses();
long* expression();

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
    if(atom(x) == t) return nil;
    else return (long*)*x;
}
long* cdr(long* x){
    if(atom(x) == t) return nil;
    else return (long*)*(x + 1);
}
long* cons(long* x, long* y){
    long* p = calloc(2, sizeof(long));
    *p = (long)x;
    *(p + 1) = (long)y;
    return p;
}

bool equal(char* a, char* b, int n){
    for(int i = 0; i < n; i++){
        if(*(a + i) != *(b + i)) return false;
    }
    return true;
}

long* number(long i){
    long* p = calloc(2, sizeof(long));
    *p = i;
    char c;
    while(true){
        c = fgetc(fp);
        if(c == ' ' || c == ')'){
            printf("%d\n",*p);
            return p;
        }
        *p *= 10;
        *p += c - '0';
    }
}

char* function(){
    char* func = calloc(5, sizeof(char));
    char c;
    for(int i = 0; i < 5; i++){
        c = fgetc(fp);
        if(c == ' ') return func;
        *(func + i) = c;
    }
}

long* parentheses(){
    char* func = function();
    if(equal(func, "eq", 2)){
        long* a = expression();
        long* b = expression();
        return eq(a, b);
    }
    else if(equal(func, "car", 3)){
        long* p = expression();
        return car(p);
    }else if(equal(func, "cdr", 3)){
        long* p = expression();
        return cdr(p);
    }
    else if(equal(func, "atom", 4)){
        long* p = expression();
        return atom(p);
    }else if(equal(func, "cons", 4)){
        long* a = expression();
        long* b = expression();
        return cons(a, b);
    }
}
long* expression(){
    char c;
    while(true){
        c = fgetc(fp);
        if(c == ' ' || c == '\n') continue;
        if(c == '(') return parentheses();
        else if(c == ')');
        else if('0' <= c && c <= '9') return number(c - '0');
        //else return string(c);
    }
}

long main(int argc, char** argv){
    t = calloc(2, sizeof(long));
    *t = 1;
    nil = calloc(2, sizeof(long));

    fp = fopen(argv[1], "r");
    printf("%d\n", *expression());
    fclose(fp);
}