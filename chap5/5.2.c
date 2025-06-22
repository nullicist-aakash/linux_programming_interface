/* Write a program that opens an existing file for writing with the O_APPEND flag, and
 * then seeks to the beginning of the file before writing some data. Where does the
 * data appear in the file? Why?
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int exercise_5_2(char* filename) {
    const int fd = open(filename, O_RDWR | O_APPEND, 0644);
    if (fd < 0) {
        perror("open");
        close(fd);
        return 1;
    }

    if (lseek(fd, 0, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return 1;
    }

    if (write(fd, "123", 3) < 0) {
        perror("write");
        close(fd);
        return 1;
    }

    char BUFF[1024];
    ssize_t len;
    while ((len = read(fd, BUFF, sizeof(BUFF))) != 0) {
        if (len < 0) {
            perror("read");
            close(fd);
            return 1;
        }

        write(1, BUFF, len);
    }

    close(fd);
    return 0;
}

/*
 * RESULT: If the file is opened in APPEND_MODE, irrespective of lseek, when write occurs, the pointer is moved to end of file.
 */