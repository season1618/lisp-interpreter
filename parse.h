#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include "bool.h"
#include "basic.h"

FILE* fp;

long* number(long i);
long* symbol(char c);
long* parentheses();
long* expr();