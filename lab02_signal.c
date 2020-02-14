
#include <stdio.h>
#include <stdlib.h>

volatile sig_atomic_t usr1Happened = 0; //indicate whether or not the program has received the signal

void signal_handler(int SIG)
{
	usr1Happened = 1; //to indicate that the signal has happened
	printf("PID= %d: Received USR1 \n", getpid());
}

int main(void) {

	struct sigaction sa;

	usr1Happened = 0;

	sa.sa_handler = signal_handler;
	sa.sa_flags= 0;
	sigemptyset(&sa.sa_mask);

	if(sigaction(SIGUSR1, &sa, NULL) == -1)
	{
		perror("Error");
		exit(1);
	}

	while(!usr1Happened){ //loop until the signal is received
		printf("PID= %d: Running... \n", getpid());
		sleep(2);
	}

	printf("PID= %d: Exiting. \n", getpid());

	return EXIT_SUCCESS;
}
