#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 100

int main (int argc, char *argv[])
{
	int child;
	int pfd[2];
	int numRead;
	char buf[BUF_SIZE];
	printf ("====>START\n");
	pipe(pfd);
	switch (child=fork()) {
	case 0:
		// Child
		close(pfd[1]); // Close write fd in child
		printf("\n\n");
		for (;;) {
			numRead = read(pfd[0], buf, BUF_SIZE);
			if (numRead <= 0) break;
			write(STDOUT_FILENO, buf, numRead);	
		}
		printf("\n\n");
		close(pfd[0]);
		break;
	default:
		// Parent
		close(pfd[0]); // Close read fd in parent
		if (argc > 1) {
			write(pfd[1], argv[1], strlen(argv[1]));
		}
		close(pfd[1]);
		break;
	}
	printf ("====> END (%d)\n", child); 
	usleep(100);
	return 0;
}

