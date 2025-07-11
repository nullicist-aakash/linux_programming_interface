/*
 * Write a program to see what happens if we try to longjmp() into a function that has
 * already returned
 */

#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

void foo() {
    switch (setjmp(env)) {
        case 0:
            printf("Normal execution\n");
            return;
        default:
            printf("Came back from somewhere\n");
            break;
    }

    printf("Came back\n");
}

void goo() {
    int x = 1;
    printf("Inside goo %d\n", x);
}

int exercise_6_2() {
    foo();
    goo();
    longjmp(env, 3);
}

// Got error of SIGBUS.