#include <iostream>
#include <pwd.h>
#include <unistd.h>

passwd* my_getpwnam(const char* name);

int main(int argc, char** argv) {
    std::cout << getpwuid(getuid())->pw_name << std::endl;
}
