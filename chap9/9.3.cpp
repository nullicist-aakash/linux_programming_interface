// Implement initgroups() using setgroups() and library functions for retrieving
// information from the password and group files (Section 8.4). Remember that a
// process must be privileged in order to be able to call setgroups().

#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <climits>
#include <cstdlib>
#include <cstring>

namespace chap9 {
    int initgroups(const char *user, gid_t group) {
        auto size = NGROUPS_MAX + 2;
        auto groups = static_cast<gid_t *>(calloc(size, sizeof(gid_t)));
        decltype(size) cur_groups = 0;

        // Step 1: Get group list for the user by iterating over group file.
        setgrent();

        struct group *_group = nullptr;
        while ((_group = getgrent()) != nullptr) {
            for (char** mem = _group->gr_mem; *mem; ++mem) {
                if (strcmp(*mem, user) == 0) {
                    groups[cur_groups++] = _group->gr_gid;
                    break;
                }
            }
        }

        endgrent();

        // Step 2: Add group parameter if it is not present
        bool group_present = false;
        for (decltype(cur_groups) i = 0; i < cur_groups; ++i) {
            if (groups[i] == group) {
                group_present = true;
                break;
            }
        }

        if (!group_present) {
            groups[cur_groups++] = group;
        }

        // Step 3: call setgroups
        auto ret = setgroups(cur_groups, groups);
        free(groups);
        return ret;
    }
}
