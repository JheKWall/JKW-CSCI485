#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* Global Variables */
pthread_mutex_t mutex_value;
int value = 0;


void *threadOne(void *ptr)
{
	// pthread_lock_mutex checks if mutex is locked:
	/// If it is, waits until unlocked.
	/// If it isnt, locks it and proceeds.
	// checks value again to ensure we dont increment over 50.
	// unlocks mutex and sleeps for a random time.
	// exits when value == 50

	while (value < 50) {
		pthread_mutex_lock(&mutex_value);
		if (value < 50) {
			value++;
			printf("\nThread One has incremeneted value to: %d", value);
		}
		pthread_mutex_unlock(&mutex_value);
		sleep(0.01);
	}
	pthread_exit(0);
}

void *threadTwo(void *ptr)
{
	// pthread_lock_mutex checks if mutex is locked:
	/// If it is, waits until unlocked.
	/// If it isnt, locks it and proceeds.
	// checks value again to ensure we dont increment over 50.
	// unlocks mutex and sleeps for a random time.
	// exits when value == 50
	
	while (value < 50) {
		pthread_mutex_lock(&mutex_value);
		if (value < 50) {
			value++;
			printf("\nThread Two has incremeneted value to: %d", value);
		}
		pthread_mutex_unlock(&mutex_value);
		sleep(0.01);
	}
	pthread_exit(0);
}

int main(void)
{
	// initialize mutex
	// create threadOne, threadTwo
	// increment number in mutex from 0 to 50 using threads
	// when number is 50, done

	pthread_mutex_init(&mutex_value, NULL);

	pthread_t threadOne_tid, threadTwo_tid;
	int count = 1;
	pthread_create(&threadOne_tid, NULL, threadOne, NULL);
	pthread_create(&threadTwo_tid, NULL, threadTwo, NULL);

	pthread_join(threadOne_tid, NULL);
	pthread_join(threadTwo_tid, NULL);

	if (value == 50) {
		printf("\nThe threads have successfully incremented the value to 50.\n");
	}
	else {
		printf("\nThe threads failed to increment the value to 50.\nInstead the value is: %d\n", value);
	}

	return 0;
}