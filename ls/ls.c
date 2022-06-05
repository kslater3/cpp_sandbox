
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <errno.h>


struct ls_line {
    char permissions[11];
    char name[256];
    off_t size;
    char lastmod[100];
    uid_t userid;
    gid_t groupid;
};



int main(int argc, char* argv[]) {
	if(argc > 2) {
		printf("Usage: ls [filename]\n");

		return 1;
	}


	size_t cwd_size = 10;
	size_t cwd_max = 1000;
	char *cwd_buf = (char *) malloc(cwd_size);
	char *cwd = getcwd(cwd_buf, cwd_size);

	// If the directory path buffer isn't big enough do a linear search for length that fits
	while(cwd == NULL && cwd_size < cwd_max && errno == ERANGE) {
		free(cwd_buf);

		cwd_size += cwd_size/2;

		cwd_buf = (char *) malloc(cwd_size);

		cwd = getcwd(cwd_buf, cwd_size);
	}

	if(cwd == NULL && errno != ERANGE) {
		perror("Internal Error");

        free(cwd_buf);

		return errno;
	}else if(cwd_size >= cwd_max) {
		printf("Error: Path exceeds MAX path size of 1000\n");

        free(cwd_buf);

		return ERANGE;
	}

	printf("Current Working Directory: %s\n\n", cwd_buf);

    printf("Permissions  Name  Size  LastMod  UID  GID\n");


	DIR *cwd_dir = opendir(cwd_buf);

	if(cwd_dir == NULL) {
		perror("Internal Error");

        free(cwd_buf);

		return errno;
	}

    long cwd_start = telldir(cwd_dir);

    if(cwd_start < 0) {
        perror("Internal Error");

        return errno;
    }

	struct dirent *cwd_node = NULL;

    // Go through and count how many entries are in the directory
    int cwd_len = 0;

    do {
        cwd_node = readdir(cwd_dir);

        if(cwd_node != NULL) {
            if(strcmp(cwd_node->d_name, ".") != 0 && strcmp(cwd_node->d_name, "..") != 0) {
                cwd_len += 1;
            }
        }
	} while(cwd_node != NULL);

    seekdir(cwd_dir, cwd_start);


    int stat_result;

    struct stat *stat_buf = (struct stat *) malloc(sizeof(struct stat));

    struct ls_line entries[cwd_len];

    int node_i = 0;

	do {
		cwd_node = readdir(cwd_dir);

		if(cwd_node != NULL) {
            if(strcmp(cwd_node->d_name, ".") != 0 && strcmp(cwd_node->d_name, "..") != 0) {
                stat_result = stat(cwd_node->d_name, stat_buf);

                if(stat_result < 0) {
                    perror("Internal Error");

                    free(cwd_buf);
                    free(stat_buf);

                    return errno;
                }


                switch (stat_buf->st_mode & S_IFMT) {
                    case S_IFBLK:
                        entries[node_i].permissions[0] = 'b';
                        break;

                    case S_IFCHR:
                        entries[node_i].permissions[0] = 'c';
                        break;

                    case S_IFDIR:
                        entries[node_i].permissions[0] = 'd';
                        break;

                    case S_IFIFO:
                        entries[node_i].permissions[0] = 'p';
                        break;

                    case S_IFLNK:
                        entries[node_i].permissions[0] = 'l';
                        break;

                    case S_IFREG:
                        entries[node_i].permissions[0] = '-';
                        break;

                    case S_IFSOCK:
                        entries[node_i].permissions[0] = 's';
                        break;

                    default:
                        entries[node_i].permissions[0] = 'u';
                        break;
                }

                switch ((stat_buf->st_mode & 0700) >> 6) {
                    case 0:
                        entries[node_i].permissions[1] = '-';
                        entries[node_i].permissions[2] = '-';
                        entries[node_i].permissions[3] = '-';
                        break;

                    case 1:
                        entries[node_i].permissions[1] = '-';
                        entries[node_i].permissions[2] = '-';
                        entries[node_i].permissions[3] = 'x';
                        break;

                    case 2:
                        entries[node_i].permissions[1] = '-';
                        entries[node_i].permissions[2] = 'w';
                        entries[node_i].permissions[3] = '-';
                        break;

                    case 3:
                        entries[node_i].permissions[1] = '-';
                        entries[node_i].permissions[2] = 'w';
                        entries[node_i].permissions[3] = 'x';
                        break;

                    case 4:
                        entries[node_i].permissions[1] = 'r';
                        entries[node_i].permissions[2] = '-';
                        entries[node_i].permissions[3] = '-';
                        break;

                    case 5:
                        entries[node_i].permissions[1] = 'r';
                        entries[node_i].permissions[2] = '-';
                        entries[node_i].permissions[3] = 'x';
                        break;

                    case 6:
                        entries[node_i].permissions[1] = 'r';
                        entries[node_i].permissions[2] = 'w';
                        entries[node_i].permissions[3] = '-';
                        break;

                    case 7:
                        entries[node_i].permissions[1] = 'r';
                        entries[node_i].permissions[2] = 'w';
                        entries[node_i].permissions[3] = 'x';
                        break;

                    default:
                        entries[node_i].permissions[1] = '?';
                        entries[node_i].permissions[2] = '?';
                        entries[node_i].permissions[3] = '?';
                        break;
                }

                switch ((stat_buf->st_mode & 0070) >> 3) {
                    case 0:
                        entries[node_i].permissions[4] = '-';
                        entries[node_i].permissions[5] = '-';
                        entries[node_i].permissions[6] = '-';
                        break;

                    case 1:
                        entries[node_i].permissions[4] = '-';
                        entries[node_i].permissions[5] = '-';
                        entries[node_i].permissions[6] = 'x';
                        break;

                    case 2:
                        entries[node_i].permissions[4] = '-';
                        entries[node_i].permissions[5] = 'w';
                        entries[node_i].permissions[6] = '-';
                        break;

                    case 3:
                        entries[node_i].permissions[4] = '-';
                        entries[node_i].permissions[5] = 'w';
                        entries[node_i].permissions[6] = 'x';
                        break;

                    case 4:
                        entries[node_i].permissions[4] = 'r';
                        entries[node_i].permissions[5] = '-';
                        entries[node_i].permissions[6] = '-';
                        break;

                    case 5:
                        entries[node_i].permissions[4] = 'r';
                        entries[node_i].permissions[5] = '-';
                        entries[node_i].permissions[6] = 'x';
                        break;

                    case 6:
                        entries[node_i].permissions[4] = 'r';
                        entries[node_i].permissions[5] = 'w';
                        entries[node_i].permissions[6] = '-';
                        break;

                    case 7:
                        entries[node_i].permissions[4] = 'r';
                        entries[node_i].permissions[5] = 'w';
                        entries[node_i].permissions[6] = 'x';
                        break;

                    default:
                        entries[node_i].permissions[4] = '?';
                        entries[node_i].permissions[5] = '?';
                        entries[node_i].permissions[6] = '?';
                        break;
                }

                switch (stat_buf->st_mode & 0007) {
                    case 0:
                        entries[node_i].permissions[7] = '-';
                        entries[node_i].permissions[8] = '-';
                        entries[node_i].permissions[9] = '-';
                        break;

                    case 1:
                        entries[node_i].permissions[7] = '-';
                        entries[node_i].permissions[8] = '-';
                        entries[node_i].permissions[9] = 'x';
                        break;

                    case 2:
                        entries[node_i].permissions[7] = '-';
                        entries[node_i].permissions[8] = 'w';
                        entries[node_i].permissions[9] = '-';
                        break;

                    case 3:
                        entries[node_i].permissions[7] = '-';
                        entries[node_i].permissions[8] = 'w';
                        entries[node_i].permissions[9] = 'x';
                        break;

                    case 4:
                        entries[node_i].permissions[7] = 'r';
                        entries[node_i].permissions[8] = '-';
                        entries[node_i].permissions[9] = '-';
                        break;

                    case 5:
                        entries[node_i].permissions[7] = 'r';
                        entries[node_i].permissions[8] = '-';
                        entries[node_i].permissions[9] = 'x';
                        break;

                    case 6:
                        entries[node_i].permissions[7] = 'r';
                        entries[node_i].permissions[8] = 'w';
                        entries[node_i].permissions[9] = '-';
                        break;

                    case 7:
                        entries[node_i].permissions[7] = 'r';
                        entries[node_i].permissions[8] = 'w';
                        entries[node_i].permissions[9] = 'x';
                        break;

                    default:
                        entries[node_i].permissions[7] = '?';
                        entries[node_i].permissions[8] = '?';
                        entries[node_i].permissions[9] = '?';
                        break;
                }

                entries[node_i].permissions[10] = '\0';

                strncpy(entries[node_i].name, cwd_node->d_name, sizeof(entries[node_i].name));
                entries[node_i].size = stat_buf->st_size;
                strftime(entries[node_i].lastmod, sizeof(entries[node_i].lastmod), "%D %T", gmtime(&stat_buf->st_mtim.tv_sec));
                entries[node_i].userid = stat_buf->st_uid;
                entries[node_i].groupid = stat_buf->st_gid;

                printf("%s  %s  %ld  %s  %d  %d\n", entries[node_i].permissions, entries[node_i].name, entries[node_i].size, entries[node_i].lastmod, entries[node_i].userid, entries[node_i].groupid);

                node_i += 1;
            }
		}
	} while(cwd_node != NULL);


	free(cwd_buf);
    free(stat_buf);

	return 0;
}
