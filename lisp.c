#include "bool.h"
#include "basic.h"
#include "parse.h"
#include "eval.h"
#include "print.h"

int main(int argc, char** argv){
    t = calloc(2, sizeof(long));
    nil = calloc(2, sizeof(long));
    *t = 1;

    var_count = 0;
    
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