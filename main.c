#include <stdio.h>
#include <stdlib.h>

int exercise_5_3(char* filename, int num_bytes, int append_flag);

int main(int argc, char** argv) {
    if (argc > 2)
        exercise_5_3(argv[1], (int)strtol(argv[2], &argv[3], 10), argc == 3);
    else
        printf("Usage: %s filename num-bytes [x]\n", argv[0]);

    return 0;
}
