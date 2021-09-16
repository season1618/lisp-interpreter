#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

FILE* fp;

long *t, *nil;

bool equal(char* a, char* b, int n);
long* number(long i);
char* function();
long* parentheses();
long* expression();

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
        if(*a == *b && *(a + 1) == *(b + 1)) return (long*)t;
        else return (long*)nil;
    }
    else if(equal(func, "car", 3)){
        long* p = expression();
        return (long*)*p;
    }else if(equal(func, "cdr", 3)){
        long* p = expression();
        return (long*)*(p + 1);
    }
    else if(equal(func, "atom", 4)){
        long* p = expression();
        if(*(p + 1) == *nil) return (long*)t;
        else return (long*)nil;
    }else if(equal(func, "cons", 4)){
        long* a = expression();
        long* b = expression();
        long* p = calloc(2, sizeof(long));
        *p = (long)a;
        *(p + 1) = (long)b;
        return p;
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