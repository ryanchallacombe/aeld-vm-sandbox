#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <poll.h>
#include <math.h>
#include <time.h>

#define TCP_PORT "4242"
char * server_ip = "192.168.86.26";
char * slash_24 = "192.168.86.";
int pfx_count = 11;
int ip_count = 14;

/**********************************
 *	Main
 **********************************/
int main(int argc, char *argv[])
{
	int rtn_val = 0;
	int sockfd;
	int ret;
	int optval = 1;
	char ipstr[INET_ADDRSTRLEN]; // space to hold the IPv4 string
	//printf("server_ip: %s\n", server_ip);

	/************************* Build list of addresses *************************/
	int idx_str_count = 4;
	int suf_start = 25, suf_end = 28;
	//int max_ip_count = suf_end - suf_start;
	char idx_str[idx_str_count];
	char target_ip[suf_end - suf_start + 1][ip_count + 1];			// include room for null terminator
	for (int i = suf_start; i <= suf_end; i++) {
		
		if (i<10) {
			idx_str[0] = i + '0';
			idx_str[1] = '\0';
		} else if (i<100) {
			idx_str[0] = i/10 + '0';
			//printf("idx_str[0] = %c\n", idx_str[0]);
			idx_str[1] = (i % 10) + '0';
			//printf("idx_str[1] = %c\n", idx_str[1]);
			idx_str[2] = '\0';
		} else {
			idx_str[0] = (i / 100) + '0';
			idx_str[1] = (i / 10) % 10 + '0';
			idx_str[2] = (i % 10) + '0';
			idx_str[3] = '\0';
		}

		//printf("idx_str: %s\n", idx_str);
		strncpy(target_ip[i - suf_start], slash_24, pfx_count);
		strncpy(target_ip[i - suf_start] + pfx_count, idx_str, idx_str_count);
		printf("target_ip[i - suf_start]: %s\n", target_ip[i - suf_start]);
	}


	/************************* Socket *************************/
	// setup
	struct addrinfo hints;
	struct addrinfo *servinfo; // will point to the linked list of results

	// hints setup
	memset(&hints, 0, sizeof(hints)); // make sure that the struct is cleared
	hints.ai_family = AF_UNSPEC;	  // don't care if ipv4 or ipv6
	hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;	  // fill in my localhost IP for me

	// poll setup
	struct pollfd pfds[1];
	int poll_timeout_ms = 1000;

	bool connection_accepted = false;
	int loop_count = 0;

	do {

		printf("attempting connection to %s on port %s\n", target_ip[loop_count], TCP_PORT);

		// getaddrinfo call
		if ((ret = getaddrinfo(target_ip[loop_count], TCP_PORT, &hints, &servinfo)) != 0)
		{
			// log error
			printf("getaddrinfo() call error: %s\n", gai_strerror(ret));
			rtn_val = -1;
			goto DONE;
		}

		// socket call
		if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
		{
			printf("socket() call error: %s\n", strerror(errno));
			rtn_val = -1;
			goto DONE;
		}

		// set SO_REUSEADDR on a socket to true (1):
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0)
		{
			perror("setsockopt");
		}

		// set socket to non-blocking
		int flags = fcntl(sockfd, F_GETFL);
		fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

		// connect() call
		int ret = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
		if ( ret == 0 ){
			// connected immediately
			connection_accepted = true;
			inet_ntop(servinfo->ai_family, (struct sockaddr_in *)servinfo->ai_addr, ipstr, INET_ADDRSTRLEN);
			printf("*** IP addr %s accepted connection on port %s ***\n", target_ip[loop_count], TCP_PORT);		
		}

		// poll
		pfds[0].fd = sockfd;
		pfds[0].events = POLLOUT;
		pfds[0].revents = 0;
		int num_events = poll(pfds, 1, poll_timeout_ms);
		// printf("num_events = %i\n", num_events);
		// printf("pfds[0].revents = %i\n", pfds[0].revents);
		if (num_events == 0) {
			printf("poll timed out\n");
			close(sockfd);
		} else if ( pfds[0].revents & POLLOUT ){
			connection_accepted = true;
			inet_ntop(servinfo->ai_family, (struct sockaddr_in *)servinfo->ai_addr, ipstr, INET_ADDRSTRLEN);
			printf("*** IP addr %s accepted connection on port %s ***\n", target_ip[loop_count], TCP_PORT);		
		} else {
			printf("unexpected event occurred\n");	
			close(sockfd);
		}

		loop_count++;

	} while ( (!connection_accepted) & ( loop_count <= (suf_end - suf_start) ));

	sleep(2);
	

DONE:
	printf("closing connection\n");
	close(sockfd);


	return rtn_val;
}