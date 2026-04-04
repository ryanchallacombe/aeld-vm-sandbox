#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/////////////////////
//	Globals
////////////////////
const int wait_ms = 800;
useconds_t wait_us = wait_ms * 1000;

struct Account {
	int balance;
	useconds_t us_wait_to_obtain_mutex;
	useconds_t us_wait_to_release_mutex;
	int withdraw_amount;
	pthread_mutex_t mutex;
	bool thread_complete_success;
};

/////////////////////
//	Functions
////////////////////

void* threadfunc(void* thread_param)
{

	//struct thread_data* thread_func_args = (struct thread_data *) thread_param;
	struct Account* account = (struct Account *) thread_param;

	const pthread_t me = pthread_self ();
	printf ("TID: %lu\n", (unsigned long) me);

	// waiting for predetermined time
	usleep(account->us_wait_to_obtain_mutex);

	// obtain mutex
	int ret;
	ret = pthread_mutex_lock(&account->mutex);
	if (ret != 0) {
		printf("Error obtaining mutex with code %d\n", ret);
	}
	
	// using the right arrow selection operator below
	// this is equivalent to saying: (*account).balance
	const int balance = account->balance;		
	const int amount = account->withdraw_amount;
	
	if (balance < amount) {		// not enough money in the bank
		pthread_mutex_unlock (&account->mutex);
		return thread_param;
	}

	account->balance = balance - amount;

	// waiting for predetermined time
	usleep(account->us_wait_to_release_mutex);
	
	pthread_mutex_unlock(&account->mutex);

	account->thread_complete_success = true;
	
	printf("New account balance: %d\n", account->balance);
	printf("Thread completed successfully: %d\n", account->thread_complete_success);

	return thread_param;
}

int main (void)
{
	printf("*** Starting program ***\n");

	// Create struct and allocate memory
	struct Account *account; 
	account = malloc( sizeof( struct Account ));
	if (account == NULL) {
		printf("malloc error. Exiting\n");
		return -1;
	}

	// initialize values
	account->balance = 500;
	account->withdraw_amount = 100;
	account->us_wait_to_obtain_mutex = 500000;
	account->us_wait_to_release_mutex = 500000;
	account->thread_complete_success = false;


	// start thread with threadfunc as start point
	pthread_t thread;		// if the thread is successfully started this will hold the thread ID 
	int ret;   				// if the thread is successfully started this will be 0
	void *ret_threadfunc;	// will store the value returned by threadfunc

	ret = pthread_create(&thread, NULL, threadfunc, account);
	if (ret != 0) {
		printf("pthread_create failed with error %d creating thread\n", ret);
		return -1;
	}

	pthread_join(thread, &ret_threadfunc);

	printf("**** New account balance: %d\n", account->balance);
	printf("**** Thread completed successfully: %d\n", account->thread_complete_success);

	// free the struct memory
	printf("**** Freeing memory\n");
	free(ret_threadfunc);
	printf("**** Memory freed\n");


	return 0;
}
