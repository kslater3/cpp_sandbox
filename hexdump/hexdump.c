
#include <stdio.h>
#include <errno.h>


#define MAX_READ 256


int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: hexdump filename\n");

        return 1;
    }


    FILE *stream = fopen(argv[1], "r");

    if(stream == NULL) {
        perror("Internal Error");

        return errno;
    }


    size_t nitems;
    char buf[MAX_READ];

    size_t linesize = 16;

    while((nitems = fread(buf, 1, MAX_READ, stream)) == MAX_READ) {
        for(size_t i = 0; i < MAX_READ/linesize - 1; i++) {
            for(size_t j = 0; j < linesize; j++) {
                printf("%x ", buf[linesize*i + j]);
            }

            printf(" :: ");

            for(size_t j = 0; j < linesize; j++) {
                if(buf[linesize*i + j] == '\n') {
                    printf("%s", "\\n");
                }else {
                    printf("%c", buf[linesize*i + j]);
                }
            }

            printf("\n");
        }


        if(MAX_READ % linesize != 0) {
            for(size_t i = 0; i < MAX_READ % linesize; i++) {
                printf("%x ", buf[MAX_READ/linesize * (linesize - 1) + i]);
            }

            printf(" :: ");

            for(size_t i = 0; i < MAX_READ % linesize; i++) {
                if(buf[MAX_READ/linesize * (linesize - 1) + i] == '\n') {
                    printf("%s", "\\n");
                }else {
                    printf("%c", buf[MAX_READ/linesize * (linesize - 1) + i]);
                }
            }

            printf("\n");
        }
    }

    buf[nitems] = '\0';
/*
    // Most of the last iteration of the fread while loop won't end on a multiple of MAX_READ so there will be a little
    // left over.
    if(nitems < MAX_READ) {
        for(size_t i = 0; i < nitems/linesize - 1; i++) {
            for(size_t j = 0; j < linesize; j++) {
                printf("%x ", buf[linesize*i + j]);
            }

            printf(" :: ");

            for(size_t j = 0; j < linesize; j++) {
                if(buf[linesize*i + j] == '\n') {
                    printf("%s", "\\n");
                }else {
                    printf("%c", buf[linesize*i + j]);
                }
            }

            printf("\n");
        }


        for(size_t i = 0; i < nitems % linesize; i++) {
            printf("%x ", buf[nitems/linesize * (linesize - 1) + i]);
        }

        printf(" :: ");

        for(size_t i = 0; i < nitems % linesize; i++) {
            if(buf[nitems/linesize * (linesize - 1) + i] == '\n') {
                printf("%s", "\\n");
            }else {
                printf("%c", buf[nitems/linesize * (linesize - 1) + i]);
            }
        }
    }
*/


    fclose(stream);

    return 0;
}
