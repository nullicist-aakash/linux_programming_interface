#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>

#define SG_SIZE (NGROUPS_MAX + 1)

namespace chap9 {
    int initgroups(const char *user, gid_t group);
}

int main(int argc, char** argv) {
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t suppGroups[SG_SIZE];

    std::cout << chap9::initgroups("tcpdump", 107) << std::endl;

    if (getresuid(&ruid, &euid, &suid) == -1)
        perror("getresuid");
    if (getresgid(&rgid, &egid, &sgid) == -1)
        perror("getresgid");

    std::cout << "ruid: " << ruid << std::endl;
    std::cout << "euid: " << euid << std::endl;
    std::cout << "suid: " << suid << std::endl;
    std::cout << "rgid: " << rgid << std::endl;
    std::cout << "egid: " << egid << std::endl;
    std::cout << "ssgid: " << sgid << std::endl;

    auto numGroups = getgroups(SG_SIZE, suppGroups);
    printf("Supplementary groups (%d): ", numGroups);
    for (auto j = 0; j < numGroups; j++) {
        std::cout << suppGroups[j] << std::endl;
    }
}
