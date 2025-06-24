#include <stdio.h>
#include <stdlib.h>

int exercise_6_3_setenv(const char *name, const char *value, int overwrite);
int exercise_6_4_unsetenv(const char *name);

int main(int argc, char** argv) {
    printf("%d\n", getenv("abc") == NULL);
    exercise_6_3_setenv("abc", "def", 0);
    printf("%s\n", getenv("abc"));
    exercise_6_3_setenv("abc", "ghi", 1);
    printf("%s\n", getenv("abc"));
    exercise_6_4_unsetenv("abc");
    printf("%d\n", getenv("abc") == NULL);
    return 0;
}
