
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>

#include <sys/types.h>
#include <errno.h>

#include <iostream>


int main(int argc, char* argv[]) {
	if(argc != 2) {
		std::cout << "Usage: find filename\n"; 

		return 1;
	}


	std::cout << "Searching for " << argv[1] << " . . .\n\n";

	
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

		return errno;
	}else if(cwd_size >= cwd_max) {
		std::cout << "Error: Path exceeds MAX path size of 1000\n";

		return ERANGE;
	}

	std::cout << "Current Working Directory: " << cwd_buf << "\n\n";


	DIR *cwd_dir = opendir(cwd_buf);

	if(cwd_dir == NULL) {
		perror("Internal Error");

		return errno;
	}

	struct dirent *cwd_node = NULL;

	std::cout << "Files in CWD: \n";

	do {
		cwd_node = readdir(cwd_dir);

		if(cwd_node != NULL) {
			std::cout << "    " << cwd_node->d_ino << "    " << cwd_node->d_name << "    " << cwd_node->d_reclen << "\n";
		} 
	} while(cwd_node != NULL);
	

	free(cwd_buf);	

	return 0;
}

