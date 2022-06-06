
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include <stdio.h>


#define MAX_READ 256


int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: cp oldfile newfile\n");

        return 1;
    }


    int oldfile_fd = open(argv[1], O_RDONLY);
    int newfile_fd = open(argv[2], O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

    if(oldfile_fd < 0 || newfile_fd < 0) {
        perror("Internal Error");

        if(oldfile_fd > 0) {
            close(oldfile_fd);
        }

        return errno;
    }


    char buf[MAX_READ];
    ssize_t items_read, items_wrote;

    while((items_read = read(oldfile_fd, buf, MAX_READ)) > 0) {
        items_wrote = write(newfile_fd, buf, items_read);

        if(items_wrote < 0) {
            perror("Internal Error");

            close(oldfile_fd);
            close(newfile_fd);

            return errno;
        }
    }

    if(items_read < 0) {
        perror("Internal Error");

        close(oldfile_fd);
        close(newfile_fd);

        return errno;
    }


    close(oldfile_fd);
    close(newfile_fd);

    return 0;
}
