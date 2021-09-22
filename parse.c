#include "bool.h"
#include "parse.h"

int parentheses_count = 0;
int line_number = 0;

long* number(long i){
    long* p = calloc(2, sizeof(long));
    *p = i;
    char c;
    while(true){
        c = fgetc(fp);
        if(c == '(') parentheses_count++;
        else if(c == ')') parentheses_count--;
        else if(c == '\n') line_number++;
        if(c == '(' || c == ')' || c == ' ' || c == '\n') return p;
        *p *= 10;
        *p += c - '0';
    }
}

long* symbol(char c){
    long* begin = calloc(2, sizeof(long));
    long* p = begin;
    long* q = calloc(2, sizeof(long));
    *q = (long)c;
    *p = (long)q;
    while(true){
        c = fgetc(fp);
        if(c == '(') parentheses_count++;
        else if(c == ')') parentheses_count--;
        else if(c == '\n') line_number++;
        if(c == '(' || c == ')' || c == ' ' || c == '\n'){
            *(p + 1) = (long)nil;
            return begin;
        }
        long* q = calloc(2, sizeof(long));
        long* r = calloc(2, sizeof(long));
        *r = (long)c;
        *q = (long)r;
        *(p + 1) = (long)q;
        p = q;
    }
}

long* parentheses(){
    long *begin = calloc(2, sizeof(long));
    long *p, *q; p = begin;
    int cnt = parentheses_count - 1;
    while(true){
        *p = (long)expr();
        if(eq(p, nil) == t){
            return begin;
        }else{
            q = calloc(2, sizeof(long));
            *(p + 1) = (long)q;
            p = q;
        }
        if(cnt == parentheses_count) return begin;
    }
}

long* expr(){
    char c;
    while(!feof(fp)){
        c = fgetc(fp);
        if(c == EOF) return nil;
        if(c == ' ') continue;
        if(c == '\n'){
            line_number++;
            continue;
        }
        if(c == '('){
            parentheses_count++;
            return parentheses();
        }else if(c == ')'){
            parentheses_count--;
            if(parentheses_count < 0){
                printf("error %d : parnetheses don't correspond.\n", line_number);
            }
            return (long*)0;
        }
        else if('0' <= c && c <= '9') return number(c - '0');
        else return symbol(c);
    }
}