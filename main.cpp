#include <iostream>

int exercise_4_1(int argc, char* argv[]);

int main() {
    char* args[] = {"tee", "-a", "/Users/aakashgupta/Documents/linux_programming_interface/cache/data.txt" };
    exercise_4_1(3, args);
    return 0;
}
