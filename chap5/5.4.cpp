/* Implement dup() and dup2() using fcntl() and, where necessary, close(). (You may
 * ignore the fact that dup2() and fcntl() return different errno values for some error
 * cases.)
 *
 * For dup2(), remember to handle the special case where oldfd equals newfd. In
 * this case, you should check whether oldfd is valid, which can be done by, for example,
 * checking if fcntl(oldfd, F_GETFL) succeeds. If oldfd is not valid, then the function
 * should return –1 with errno set to EBADF.
*/

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int exercise_5_4_dup(int fd) {
    return fcntl(fd, F_DUPFD, 0);
}

int exercise_5_4_dup2(int oldfd, int newfd) {
    if (fcntl(oldfd, F_GETFL) == -1) {
        errno = EBADF;
        return -1;
    }

    if (oldfd == newfd)
        return newfd;

    close(newfd);
    return fcntl(oldfd, F_DUPFD, newfd);
}
