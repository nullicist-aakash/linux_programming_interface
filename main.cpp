#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>
#include <vector>

#define SG_SIZE (NGROUPS_MAX + 1)

namespace chap12 {
    struct id_and_cmd {
        pid_t pid;
        std::string command;
    };

    std::vector<id_and_cmd> get_id_and_cmd(char* name);
}

int main(int argc, char** argv) {
    for (auto &[pid,cmd] : chap12::get_id_and_cmd(NULL)) {
        std::cout << pid << "\t" << cmd << std::endl;
    }
    sleep(10000);
}
