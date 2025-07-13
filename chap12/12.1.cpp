#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <pwd.h>
#include <vector>
#include <string>
#include <optional>

#define MAX_SIZE 65536

namespace {

    std::optional<size_t> parse_int(char* str) {
        char* end;
        auto pid = static_cast<pid_t>(strtol(str, &end, 10));
        if (*end == '\0')
            return pid;
        return {};
    }

    uid_t userIdFromName(const char *name) {
        /* Return UID corresponding to 'name', or -1 on error */
        struct passwd *pwd;
        uid_t u;
        char *endptr;
        if (name == NULL || *name == '\0') /* On NULL or empty string */
            return -1; /* return an error */
        u = strtol(name, &endptr, 10); if (*endptr == '\0') return u;
        /* As a convenience to caller */
        /* allow a numeric string */
        pwd = getpwnam(name);
        if (pwd == NULL)
            return -1;
        return pwd->pw_uid;
    }

    ssize_t readn(int fd, void *vptr, size_t n)
    {
        size_t nleft;
        ssize_t nread;
        char *ptr;

        ptr = static_cast<char*>(vptr);
        nleft = n;

        while (nleft > 0) {
            if ((nread = read(fd, ptr, nleft)) < 0) {
                if (errno == EINTR)
                    nread = 0; /* and call read() again */
                else
                    return (-1);
            } else if (nread == 0)
                break; /* EOF */
            nleft -= nread;
            ptr += nread;
        }

        return (n - nleft); /* return >= 0 */
    }

    std::pair<uid_t, std::string> get_run_user_and_command(int fd) {
        char content[MAX_SIZE];
        if (auto actual_read = readn(fd, content, MAX_SIZE); actual_read < 0)
            return {-1, ""};

        auto command_location = strstr(content, "Name:") + sizeof("Name:");
        while (isspace(*command_location))
            ++command_location;
        auto command_end = strstr(command_location, "\n");
        auto uid_location = strstr(content, "Uid:") + sizeof("Uid:");
        while (isspace(*uid_location))
            ++uid_location;
        auto uid_end = strstr(uid_location, " ");
        *command_end = '\0';
        *uid_end = '\0';

        return {
            static_cast<uid_t>(*parse_int(uid_location)),
            command_location
        };
    }
}

namespace chap12 {
    struct id_and_cmd {
        pid_t pid;
        std::string command;
    };

    std::vector<id_and_cmd> get_id_and_cmd(char* name) {
        auto uid = name == NULL ? -2 : userIdFromName(name);
        if (uid == -1) {
            perror("getpwnam");
            return {};
        }

        char path_name[NAME_MAX + 1];
        strcpy(path_name, "/proc/");
        auto dir = opendir(path_name);
        const auto start_for_subchild = path_name + strlen(path_name);

        if (!dir) {
            perror("opendir");
            return {};
        }
        std::vector<id_and_cmd> id_and_cmds;

        // Iterate over all files
        while (true) {
            errno = 0;

            auto entry = readdir(dir);
            if (!entry) {
                if (errno == 0) {
                    break;
                }
                perror("readdir");
                auto backup = errno;
                closedir(dir);
                errno = backup;
                return {};
            }

            auto _pid = parse_int(entry->d_name);
            if (!_pid)
                continue;

            // Try to open the file, and check if it exists (race between us and closing file)
            strcpy(start_for_subchild, entry->d_name);
            strcpy(start_for_subchild + strlen(start_for_subchild), "/status");
            auto fd = open(path_name, O_RDONLY);
            if (fd == -1) {
                if (errno == ENOENT)
                    continue;       // process was closed;
                perror("open");
                closedir(dir);
                return {};
            }

            auto [run_user, command] = get_run_user_and_command(fd);
            if (uid != -2 && run_user != uid) {
                closedir(dir);
                if (close(fd) != 0) {
                    perror("close");
                    return {};
                }
                return {};
            }

            id_and_cmds.push_back(id_and_cmd
                {
                    .pid = static_cast<pid_t>(_pid.value()),
                    .command = std::move(command)
                });

            if (close(fd) != 0) {
                perror("close");
                return {};
            }
        }

        if (closedir(dir) != 0) {
            perror("dir");
            return {};
        }

        return id_and_cmds;
    }
}

// More exercises are pain. I get the idea on how to do that