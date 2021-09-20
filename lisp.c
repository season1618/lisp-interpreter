#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

FILE* fp;

long *t, *nil;

int var_count = 0;
long* var[10] = {};
long* value[10] = {};

// basic function
long* atom(long* x);
long* eq(long* x, long* y);
long* car(long* x);
long* cdr(long* y);
long* cons(long* x, long* y);

long* number(long i);
char* function();
long* parentheses();
long* expr();

bool equal(long* a, char* b);
void print(long* p);

// basic function
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
        printf("error : argument of car is pair.");
        return nil;
    }else return (long*)*x;
}
long* cdr(long* x){
    if(atom(x) == t){
        printf("error : argument of cdr is pair.");
        return nil;
    }else return (long*)*(x + 1);
}
long* cons(long* x, long* y){
    long* p = calloc(2, sizeof(long));
    *p = (long)x;
    *(p + 1) = (long)y;
    return p;
}

// compare string
bool equal(long* a, char* b){
    if(atom(a) == t) return false;
    for(int i = 0; i < 8; i++){
        if(a == nil && *b == '\0') return true;
        else if(*((long*)*a) != (long)*b) return false;
        a = (long*)*(a + 1);
        b++;
    }
}

// parse
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

long* eval(long* p){
    if(atom(p) == t) return p;
    for(int i = var_count - 1; i >= 0; i--){
        if(eq(p, var[i]) == t) return value[i];
    }

    long* token = car(p);
    long* args = cdr(p);

    // basic function
    if(equal(token, "atom")) return atom(car(args));
    else if(equal(token, "eq")) return eq(eval(car(args)), eval(car(cdr(args))));
    else if(equal(token, "car")) return car(eval(car(args)));
    else if(equal(token, "cdr")) return cdr(eval(car(args)));
    else if(equal(token, "cons")) return cons(eval(car(args)), eval(car(cdr(args))));

    // special form
    if(equal(token, "cond")){
        while(eq(args, nil) == nil){
            long* condition = car(car(args));
            long* expression = car(cdr(car(args)));
            if(eval(condition) == t) return eval(expression);
            args = cdr(args);
        }
        return nil;
    }
    else if(equal(token, "lambda")){
        return args;
    }
    else if(equal(token, "define")){
        var[var_count] = car(args);
        value[var_count] = eval(car(cdr(args)));
        var_count++;
        return value[var_count - 1];
    }
    
    bool is_lambda = false;
    for(int i = var_count - 1; i >= 0; i--){
        if(eq(token, var[i]) == t){
            token = value[i];
            is_lambda = true;
            break;
        }
    }
    if(!is_lambda){
        return cons(token, eval(args));
    }
    // lambda expression
    if(is_lambda){
        long* params = car(token);
        int n = 0;
        while(eq(params, nil) == nil){
            var[var_count] = car(params);
            value[var_count] = eval(car(args));
            var_count++;
            n++;
            params = cdr(params);
            args = cdr(args);
        }
        long* ret = eval(car(cdr(token)));
        for(int i = 0; i < n; i++){
            var_count--;
            var[var_count] = 0;
            value[var_count] = 0;
        }
        return ret;
    }
}

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

int main(int argc, char** argv){
    t = calloc(2, sizeof(long));
    nil = calloc(2, sizeof(long));
    *t = 1;

    fp = fopen(argv[1], "r");
    while(!feof(fp)){
        long* begin = expr();
        //print(begin);
        //printf("\n");
        print(eval(begin));
        printf("\n");
    }
    fclose(fp);
}