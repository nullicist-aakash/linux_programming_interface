/* Implement readv() and writev() using read(), write(), and suitable functions from the
 * malloc package (Section 7.1.2).
 */

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/posix1_lim.h>
#include <sys/uio.h>

ssize_t exercise_5_7_writev(int fd, const struct iovec *iov, int iovcnt) {
    if (iovcnt < 0) {
        errno = EINVAL;
        return -1;
    }

    size_t final_len = 0;

    for (int i = 0; i < iovcnt; ++i) {
        const ssize_t writen = write(fd, iov[i].iov_base, iov[i].iov_len);
        if (writen < 0)
            return writen;

        final_len += writen;

        // If on intermediate failures, we return -1 with partial operation, it seems
        // fair that we can merge the final length check inside this loop itself.
        if (final_len > SSIZE_MAX) {
            errno = EINVAL;
            return -1;
        }

        if (writen != iov[i].iov_len)
            return (ssize_t)final_len;
    }

    return (ssize_t)final_len;
}

ssize_t exercise_5_7_readv(int fd, const struct iovec *iov, int iovcnt) {
    if (iovcnt < 0) {
        errno = EINVAL;
        return -1;
    }

    size_t final_len = 0;
    for (int i = 0; i < iovcnt; ++i) {
        const ssize_t readn = read(fd, iov[i].iov_base, iov[i].iov_len);
        if (readn < 0)
            return readn;

        final_len += readn;

        // If on intermediate failures, we return -1 with partial operation, it seems
        // fair that we can merge the final length check inside this loop itself.
        if (final_len > SSIZE_MAX) {
            errno = EINVAL;
            return -1;
        }

        if (readn != iov[i].iov_len)
            return (ssize_t)final_len;

    }

    return (ssize_t)final_len;
}
