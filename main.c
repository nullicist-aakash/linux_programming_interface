int exercise_5_2(char* filename);

int main() {
    char* args[] = {"cpp", "/IdeaProjects/linux_programming_interface/cache/source.txt", "/IdeaProjects/linux_programming_interface/cache/destination.txt" };
    exercise_5_2(args[1]);
    return 0;
}
