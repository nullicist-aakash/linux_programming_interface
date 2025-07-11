/*
 * The tee command reads its standard input until end-of-file, writing a copy of the input
 * to standard output and to the file named in its command-line argument. (We show
 * an example of the use of this command when we discuss FIFOs in Section 44.7.)
 * Implement tee using I/O system calls. By default, tee overwrites any existing file with
 * the given name. Implement the –a command-line option (tee –a file), which causes tee
 * to append text to the end of a file if it already exists. (Refer to Appendix B for a
 * description of the getopt() function, which can be used to parse command-line
 * options.)
*/

#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

int exercise_4_1(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-a] file\n", argv[0]);
        return 1;
    }

    int is_append_mode = 0;

    if (int option; (option = getopt(argc, argv, "a")) != -1) {
        if (option == 'a')
            is_append_mode = 1;
        else {
            fprintf(stderr, "Usage: %s [-a] file\n", argv[0]);
            return 1;
        }
    }

    int file_fd;
    if (is_append_mode) {
        file_fd = open(argv[optind], O_WRONLY | O_APPEND | O_CREAT, 0644);
    } else {
        file_fd = open(argv[optind], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    }

    if (file_fd < 0) {
        perror("open");
        return 1;
    }

    char BUFFER[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, BUFFER, sizeof(BUFFER))) > 0) {
        if (write(STDOUT_FILENO, BUFFER, bytes_read) < 0) {
            perror("write to stdout");
            close(file_fd);
            return 1;
        }
        if (write(file_fd, BUFFER, bytes_read) < 0) {
            perror("write to file");
            close(file_fd);
            return 1;
        }
    }
    if (bytes_read < 0) {
        perror("read");
    }
    close(file_fd);

    return 0;
}
