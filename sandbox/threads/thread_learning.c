#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/////////////////////
//	Globals
////////////////////
static pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;

/////////////////////
//	Functions
////////////////////
void * start_thread (void *message)
{
	pthread_mutex_lock(&the_mutex);

	const pthread_t me = pthread_self ();
	printf ("Message: %s with TID: %d\n", (const char *) message, (int) me);

	pthread_mutex_unlock(&the_mutex);

	return message;
}


int main (void)
{

	pthread_t thing1, thing2;

	const char *message1 = "Thing 1";
	const char *message2 = "Thing 2";

	/* Create two threads, each with a different message. */
	pthread_create (&thing1, NULL, start_thread, (void *) message1);
	pthread_create (&thing2, NULL, start_thread, (void *) message2);
	/*
	* Wait for the threads to exit. If we didn't join here,
	* we'd risk terminating this main thread before the
	* other two threads finished.
	*/
	pthread_join (thing1, NULL);
	pthread_join (thing2, NULL);

	return 0;
}

/*
static pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
int withdraw (struct account *account, int amount)
{
pthread_mutex_lock (&the_mutex);
const int balance = account->balance;
if (balance < amount) {
pthread_mutex_unlock (&the_mutex);
return -1;
}
account->balance = balance - amount;
pthread_mutex_unlock (&the_mutex);
disburse_money (amount);
return 0;
}
*/