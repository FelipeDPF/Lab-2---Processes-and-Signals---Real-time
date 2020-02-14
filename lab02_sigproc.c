#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t usr1Happened = 0; //indicate whether or not the program has received the signal

void signal_handler(int SIG)
{
	usr1Happened = 1; //to indicate that the signal has happened
}

int main(void) {

	pid_t pid;
	int i;
	int numChildren=0;
	int reportStatus=0;
	int totalNumber= 0;

	struct sigaction sa;
	usr1Happened = 0;
	sa.sa_handler = signal_handler;
	sa.sa_flags= 0;
	sigemptyset(&sa.sa_mask);

	printf("Enter the number of children: \n");
	scanf(" %d", &numChildren);

	totalNumber= numChildren + 1; // number of children + parent

	if(sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("Error");
		exit(1);
	}

	for(i = 0; i < numChildren; i++){

		switch (pid = fork()) {
		case -1:

			perror("fork"); /* something went wrong */
			exit(1); /* parent exits */

		case 0: // children

			printf("PID = %d: Child running... \n", getpid());

			while(!usr1Happened){ //loop until the signal is received
			}

			printf("PID = %d: Child received USR1. \n", getpid());
			printf("PID = %d: Child exiting. \n", getpid());
			exit(0);

		default: // parent
			if(i == 0){
				printf("PID = %d: Parent running... \n", getpid());
			}
		}
	}

	while(totalNumber != 0){ // Loop until Parent and children have a value
		wait(&reportStatus); // wait for all children to finished
		totalNumber--;
	}


	printf("PID = %d: Children finished, parent exiting. \n", getpid());

	return EXIT_SUCCESS;
}
