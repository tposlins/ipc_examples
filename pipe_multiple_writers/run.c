#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_CHILDREN 3
#define BUF_SIZE	 100

void run_child(int *pfd, int instance, int num_args, char *arg_data[]);

// Children are the writers, parent is the reader
int main (int argc, char *argv[])
{
	int children[NUM_CHILDREN];
	int pfd[2];
	int numRead;
	int child_count = 0;
	int i;
	char buf[BUF_SIZE];
	int ppid = getpid();

	fprintf (stderr, "Parent (%d) START\n", ppid);
	pipe(pfd);
	for (i = 0; i < NUM_CHILDREN; i++) {
		children[i] = fork();
		if (children[i] == 0) {
			run_child(pfd, i, argc, argv);
		}
	}

	// Parent
	close(pfd[1]); // Close write fd in parent
	for (;;) {
		numRead = read(pfd[0], buf, BUF_SIZE);
		if (numRead <= 0) break;
		write(STDOUT_FILENO, buf, numRead);	
	}
	close(pfd[0]);

	usleep (100);
	fprintf (stderr, "Parent (%d) END\n", ppid);
	fprintf (stderr, "Parent (%d) CHILDREN PID\n", ppid);
	for (i = 0; i < NUM_CHILDREN; i++) {
		fprintf (stderr, "CHILD PID => %d\n", children[i]);
	} 
	usleep(100);
	printf("\n\n");
	return 0;
}

void run_child(int *pfd, int instance, int num_args, char *arg_data[]) {
	int arg_offset = 1 + instance;
	int i;
	int cpid = getpid();
	fprintf (stderr, "Child (%d) STARTED => %d\n", cpid, instance);
	close(pfd[0]); // Close read fd in parent
	if (num_args > arg_offset) {
		for (i = 0; i < 5; i++) {
			write(pfd[1], arg_data[arg_offset], strlen(arg_data[arg_offset]));
			usleep(1);
		}
	} else {
		fprintf (stderr, "Child (%d) - No Data to send\n", cpid);
	}
	close(pfd[1]);
	fprintf (stderr, "Child (%d) ENDED\n", cpid);
	exit(0);
}
