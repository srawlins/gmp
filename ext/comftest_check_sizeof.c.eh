#include <ruby.h>

#include <stdio.h>

int conftest_const = (int)(sizeof(intptr_t));
int main() {printf("%d\n", conftest_const); return 0;}

