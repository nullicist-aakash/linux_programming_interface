/*
 * Write a program like cp that, when used to copy a regular file that contains holes
 * (sequences of null bytes), also creates corresponding holes in the target file.
 */

// Create a hole file via this:
// dd if=/dev/urandom of=sparsefile.img bs=1M count=1         # Write first 1 MB of real data
// dd if=/dev/urandom of=sparsefile.img bs=1M count=1 seek=1025 conv=notrunc  # Write another 1 MB, after 1 GB hole

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

off_t Lseek(int fd, off_t offset, int whence) {
    off_t val;
    if ((val = lseek(fd, offset, whence)) == -1) {
        perror("lseek");
        exit(1);
    }
    return val;
}

bool all_zero(char* buff, ssize_t count) {
    if (count == 0)
        return true;

    return buff[0] == '\0' && memcmp(buff, buff + 1, count - 1) == 0;
}

int exercise_4_2(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    int read_fd = open(argv[1], O_RDONLY, 0644);
    if (read_fd < 0) {
        perror("open source file");
        return 1;
    }

    int write_fd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (write_fd < 0) {
        perror("open destination file");
        return 1;
    }

    char BUFF[1024];
    ssize_t read_size;

    long long skip_write_buff = 0;
    while ((read_size = read(read_fd, BUFF, sizeof BUFF)) > 0) {
        if (all_zero(BUFF, read_size)) {
            skip_write_buff += read_size;
            continue;
        }
        if (skip_write_buff > 0) {
            lseek(write_fd, skip_write_buff, SEEK_CUR);
            skip_write_buff = 0;
        }

        for (auto remain_write = read_size; remain_write > 0; ) {
            long write_count = 0;
            if ((write_count = write(write_fd, BUFF + read_size - remain_write, remain_write)) < 0) {
                perror("write");
                return 1;
            }
            remain_write -= write_count;
        }
    }

    if (read_size < 0) {
        perror("read");
        return 1;
    }

    close(read_fd);
    close(write_fd);
    return 0;
}
