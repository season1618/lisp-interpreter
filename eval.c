#include "bool.h"
#include "basic.h"
#include "eval.h"

bool equal(long* a, char* b){
    if(atom(a) == t) return false;
    for(int i = 0; i < 8; i++){
        if(a == nil && *b == '\0') return true;
        else if(*((long*)*a) != (long)*b) return false;
        a = (long*)*(a + 1);
        b++;
    }
}

long* eval(long* p){
    if(equal(p, "t")) return t;
    else if(equal(p, "nil")) return nil;
    for(int i = var_count - 1; i >= 0; i--){
        if(eq(p, var[i]) == t) return value[i];
    }

    long* token = car(p);
    long* args = cdr(p);

    // basic function
    if(equal(token, "atom")) return atom(eval(car(args)));
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
    else if(equal(token, "quote")){
        return car(args);
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
    else{
        // lambda expression
        for(int i = var_count - 1; i >= 0; i--){
            if(eq(token, var[i]) == t){
                token = value[i];
                break;
            }
        }
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