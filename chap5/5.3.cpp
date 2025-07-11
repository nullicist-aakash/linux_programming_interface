/*
 * This exercise is designed to demonstrate why the atomicity guaranteed by opening
 * a file with the O_APPEND flag is necessary. Write a program that takes up to three
 * command-line arguments:
 * $ atomic_append filename num-bytes [x]
 *
 * This file should open the specified filename (creating it if necessary) and append
 * num-bytes bytes to the file by using write() to write a byte at a time. By default, the
 * program should open the file with the O_APPEND flag, but if a third command-line
 * argument (x) is supplied, then the O_APPEND flag should be omitted, and instead the
 * program should perform an lseek(fd, 0, SEEK_END) call before each write(). Run
 * two instances of this program at the same time without the x argument to write
 * 1 million bytes to the same file:
 * $ atomic_append f1 1000000 & atomic_append f1 1000000
 *
 * Repeat the same steps, writing to a different file, but this time specifying the x
 * argument:
 * $ atomic_append f2 1000000 x & atomic_append f2 1000000 x
 *
 * List the sizes of the files f1 and f2 using ls –l and explain the difference.
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int exercise_5_3(char* filename, int num_bytes, int append_flag) {
    const int fd = open(filename, O_WRONLY | O_CREAT | (append_flag > 0 ? O_APPEND : 0), 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }
    char c;

    while (num_bytes-- != 0) {
        if (!append_flag)
            if (lseek(fd, 0, SEEK_END) < 0) {
                perror("lseek");
                return 1;
            }

        c = num_bytes & 0xFF;
        if (write(fd, &c, 1) < 0) {
            perror("write");
            return 1;
        }
    }

    return 0;
}


/* Results: Using lseek separately is not atomic.
 * $ ./a.out ./cache/dump.txt 1000000 &  ./a.out ./cache/dump.txt 1000000
 * [1] 25488
 * [1]+  Done                    ./a.out ./cache/dump.txt 1000000
 * $ ls -l cache/
 * total 1956
 * -rw-r--r-- 1 dev dev 2000000 Jun 22 20:04 dump.txt
 * $ rm ./cache/dump.txt
 *
 *
 * $ ./a.out ./cache/dump.txt 1000000 x & ./a.out ./cache/dump.txt 1000000 x
 * [1] 25624
 * [1]+  Done                    ./a.out ./cache/dump.txt 1000000 x
 * $ ls -l cache/
 * total 984
 * -rw-r--r-- 1 dev dev 1004882 Jun 22 20:04 dump.txt
 */