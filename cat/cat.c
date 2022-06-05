
#include <stdio.h>
#include <errno.h>


#define MAX_READ 256


int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: cat filename\n");

        return 1;
    }


    FILE *stream = fopen(argv[1], "r");

    if(stream == NULL) {
        perror("Internal Error");

        return errno;
    }


    size_t nitems;
    char buf[MAX_READ];

    while((nitems = fread(buf, 1, MAX_READ, stream)) == MAX_READ) {
        printf("%s", buf);
    }

    buf[nitems] = '\0';

    // Most of the last iteration of the fread while loop won't end on a multiple of MAX_READ so there will be a little
    // left over.
    if(nitems < MAX_READ) {
        printf("%s", buf);
    }


    fclose(stream);

    return 0;
}
