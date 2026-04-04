#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/////////////////////
//	Globals
////////////////////
static pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
const int wait_ms = 800;
useconds_t wait_us = wait_ms * 1000;

struct Account {
	int balance;
	pthread_mutex_t mutex;
};

/////////////////////
//	Functions
////////////////////
void * start_thread (void *message)
{
	pthread_mutex_lock(&the_mutex);

	const pthread_t me = pthread_self ();
	printf ("Message: %s with TID: %lu\n", (const char *) message, (unsigned long) me);

	pthread_mutex_unlock(&the_mutex);

	return message;
}

int withdraw (struct Account *account, int amount)
{
	pthread_mutex_lock (&account->mutex);

	// waiting for predetermined time
	usleep(wait_ms);
	
	// using the right arrow selection operator below
	// this is equivalent to saying: (*account).balance
	const int balance = account->balance;		
	
	if (balance < amount) {
		pthread_mutex_unlock (&account->mutex);
		return -1;
	}

	account->balance = balance - amount;
	
	pthread_mutex_unlock (&account->mutex);
	
	return 0;
}

int main (void)
{

	// Create struct and initialize values
	struct Account account = {.balance = 500, .mutex = PTHREAD_MUTEX_INITIALIZER};

	// Setup for start of thread
	int withdraw_amount = 100;

	int ret = withdraw(&account, withdraw_amount);

	if (!ret) {
		printf("Withrawal was a success\n");
		printf("New balance: %d\n", account.balance);
	}
	else {
		printf("Withrawal failed\n");
		printf("Account balance: %d\n", account.balance);
	}


	return 0;
}
