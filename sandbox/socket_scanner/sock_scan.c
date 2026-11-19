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

int try_server_connect(char *server_ip, char *tcp_port, int timeout_ms);
//void build_ipv4_addr_array(char *pfx, int suf_start, int suf_end, char *target_ip);

/**********************************
 *	Main
 **********************************/
int main(int argc, char *argv[])
{

	/************************* Build list of addresses *************************/
	char * slash_24 = "192.168.86.";
	int suf_start = 25, suf_end = 28;
	int pfx_count = 11, ip_char_count = 14;
	char target_ip[suf_end - suf_start + 1][ip_char_count + 1];			// include room for null terminator

	//build_ipv4_addr_array(slash_24, suf_start, suf_end, target_ip);

	int idx_str_count = 4;
	char idx_str[idx_str_count];
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

	int poll_timeout_ms = 1000;
	bool connection_accepted = false;
	int loop_count = 0;
	int socket_fd;

	do {
		printf("attempting connection to %s on port %s\n", target_ip[loop_count], TCP_PORT);

		if ( (socket_fd = try_server_connect(target_ip[loop_count], TCP_PORT, poll_timeout_ms)) != -1) {
			printf("*** IP addr %s accepted connection on port %s ***\n", target_ip[loop_count], TCP_PORT);	
			connection_accepted = true;
		}

		loop_count++;

	} while ( (!connection_accepted) & ( loop_count <= (suf_end - suf_start) ));

	sleep(2);

	if ( !connection_accepted ) {
		printf("failed to connect to any of the ip address / tcp ports in the list\n");
	} else {
		printf("connected. socket_fd = %i\n", socket_fd);
	}
	
	printf("closing connection\n");
	close(socket_fd);

	return 0;

}

/***********
 * try_server_connect())
 * 		inputs: server_ip = ip address to try (char)
 * 				tcp_port = port to try
 *				timeout_ms = poll timeout
 *
 * 		returns:
 * 				-1 	= failure to connect, i.e. poll timed out or error occurred
 *				else = socket fd
 */
int try_server_connect(char *server_ip, char *tcp_port, int timeout_ms) {
	// setup
	struct addrinfo hints;
	struct addrinfo *servinfo; // will point to the linked list of results
	int ret, sockfd, optval = 1;

	// hints setup
	memset(&hints, 0, sizeof(hints)); // make sure that the struct is cleared
	hints.ai_family = AF_UNSPEC;	  // don't care if ipv4 or ipv6
	hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
	hints.ai_flags = AI_PASSIVE;	  // fill in my localhost IP for me

	// poll setup
	struct pollfd pfds[1];

	// getaddrinfo call
	if ((ret = getaddrinfo(server_ip, tcp_port, &hints, &servinfo)) != 0)
	{
		// log error
		printf("getaddrinfo() call error: %s\n", gai_strerror(ret));
		return -1;
	}

	// socket call
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1)
	{
		printf("socket() call error: %s\n", strerror(errno));
		return -1;
	}

	// set SO_REUSEADDR on a socket to true (1):
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0)
	{
		perror("setsockopt");
		return -1;
	}

	// set socket to non-blocking
	int flags = fcntl(sockfd, F_GETFL);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	// connect() call
	ret = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	if ( ret == 0 ){
		// connected immediately
		return sockfd;		
	}

	// poll
	pfds[0].fd = sockfd;
	pfds[0].events = POLLOUT;
	pfds[0].revents = 0;
	int num_events = poll(pfds, 1, timeout_ms);
	if (num_events == 0) {
		// poll timed out
		close(sockfd);
		return -1;
	} else if ( pfds[0].revents & POLLOUT ){
	} else {
		// unexpected poll() event occurred
		close(sockfd);
		return -1;
	}

	return sockfd;
}

/***********
 * build_ipv4_addr_array()
 * 		inputs: 
 				char *pfx = 11 digit ip /24 prefix (char array)
 					must include period at the end, i.e. "192.168.86."
 				int suf_start = suffix (last octet of ip addr) to START with (lowest)
 				int suf_end =  suffix (last octet of ip addr) to END with (lowest)
 *
 * 		returns:
 * 				pointer to array of null terminated ip address strings
 * 
 *  				e.g. pfx = "192.168.86.", suf_start = 25, suf_end = 30
 					output is a pointer to array with range of ip addresses
 					starting at "192.168.86.25" and ending with "192.168.86.30"
 *				
 */
/*
void build_ipv4_addr_array(char *pfx, int suf_start, int suf_end, char *target_ip) {

	int pfx_count = 11, ip_char_count = 14;
	//char target_ip[suf_end - suf_start + 1][ip_char_count + 1];			// include room for null terminator

	int idx_str_count = 4;
	char idx_str[idx_str_count];
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

		strncpy(target_ip[i - suf_start], pfx, pfx_count);
		strncpy(target_ip[i - suf_start] + pfx_count, idx_str, idx_str_count);
		printf("target_ip[i - suf_start]: %s\n", target_ip[i - suf_start]);
	}

}
*/